#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif

RH_ASK driver(2000, 4, 2, 5);

int red = 7;
int green = 8;
int blue = 9;
boolean fillWater = false;

void setup() {
  pinMode(red, OUTPUT); //Red
  pinMode(green, OUTPUT); //Green
  pinMode(blue, OUTPUT); //Blue
  
  #ifdef RH_HAVE_SERIAL
    Serial.begin(9600);    // Debugging only
  #endif
    if (!driver.init())
  #ifdef RH_HAVE_SERIAL
    Serial.println("init failed");
  #else
  ;
  #endif
}

void loop() {
  //WorkOnOpen (When Sensor is getting LOW voltage)
  threeSensorModel(true);
  //WorkOnClosed (When Sensor is getting HIGH voltage)
  //threeSensorModel(false);
}

void blink(int loc) {
  digitalWrite(loc, HIGH);
  delay(1);
  digitalWrite(loc, LOW);
  delay(1);
}

void resetAllLeds() {
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(blue, LOW);
}

int count=0;
String repeatWord="begin";
String firstWord="begin";
String secondWord="begin";
  
String filter(String word, int frequency) {
  if (word=="111") {
    //Force to turn off the motor
    //Force set repeatedWord
    fillWater=false;
    repeatWord="111";
  }
  if (repeatWord=="begin") {
    repeatWord=word;
  }
  if (firstWord=="begin") {
    firstWord=word;
    return repeatWord;
  }
  else {
    secondWord = word;
  }
    
  if (firstWord==secondWord) {
    count++;
    if (count==frequency) {
      count=0;
      repeatWord=secondWord;
    }
    return repeatWord;
  }
  else {
    count=0;
    firstWord=secondWord;
    return repeatWord;
  }
}

String getSensorStatus(boolean low, boolean medium, boolean high) {
  String sensor=(high)?"1":"0";
  sensor+=(medium)?"1":"0";
  sensor+=(low)?"1":"0";
  return sensor;
}

int countError=0;

void threeSensorModel(boolean isSensorReverse) {
  boolean isFull = analogRead(0);
  boolean isTwoThird = analogRead(1);
  boolean isOneThird = analogRead(2);
  
  if (isSensorReverse) {
    isFull = !isFull;
    isTwoThird = !isTwoThird;
    isOneThird = !isOneThird;
  }

  Serial.print(isFull);
  Serial.print(isTwoThird);
  Serial.print(isOneThird);

  String sensor=getSensorStatus(isOneThird, isTwoThird, isFull);
  sensor = filter(sensor, 10); //filter changes 10 times
  //Check 1st Sensor
  //Remove resistor

  String output = "sensorFailure";

  if (sensor=="111") {
    fillWater = false;
    resetAllLeds();
    digitalWrite(green, HIGH);
    output = "stopGreen";
  }
  else if (sensor=="011") {
    if (fillWater) {
      resetAllLeds();
      blink(green);
      output = "startBlinkGreen";
    }
    else {
      resetAllLeds();
      digitalWrite(blue, HIGH);
      output = "stopBlue";
    }
  }
  else if (sensor=="001") {
    if (fillWater) {
      resetAllLeds();
      blink(blue);
      output = "startBlinkBlue";
    }
    else {
      resetAllLeds();
      digitalWrite(red, HIGH);
      output = "stopRed";
    }
  }
  else if (sensor=="000") {
    if (fillWater) {
      resetAllLeds();
      blink(red);
      output = "startBlinkRed";
    }
    else {
      resetAllLeds();
      digitalWrite(red, HIGH);
      output = "stopRed";
    }
    fillWater = true;
  }
  else if (output=="sensorFailure") {
    resetAllLeds();
    countError++;
    switch (countError) {
      case 1:
        blink(red);  
      break;
      
      case 2:
        blink(green);
      break;
      
      case 3:
        blink(blue);
        countError=0;
      break;
    }
  }
  
  sendText(output.c_str());
}

void sendText(const char *msg) {
  driver.send((uint8_t *)msg, strlen(msg));
  driver.waitPacketSent();
  Serial.println(msg);
}
