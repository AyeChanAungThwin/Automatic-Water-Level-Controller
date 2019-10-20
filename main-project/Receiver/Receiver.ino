#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h>
#endif
#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3F, 16, 2);
RH_ASK driver(2000, 2, 4, 5); 

//int sensorFailure = 6;
int red = 7;
int green = 8;
int blue = 9;
int driveRelay = 10;
int buzzer = 11;

void setup() {
  pinMode(red, OUTPUT); //Red
  pinMode(green, OUTPUT); //Green
  pinMode(blue, OUTPUT); //Blue
  //pinMode(sensorFailure, OUTPUT); //Sensor Failure Notification (White)
  pinMode(driveRelay, OUTPUT); //Drive relay
  pinMode(buzzer, OUTPUT); //Sound
  
  #ifdef RH_HAVE_SERIAL
    Serial.begin(9600);
  #endif
    if (!driver.init())
  #ifdef RH_HAVE_SERIAL
    Serial.println("init failed");
  #else
  ;
  #endif
  initLCD();
}

void initLCD() {
  lcd.begin();
  lcd.backlight();

  lcd.setCursor(1, 0);
  lcd.print("Initializing...");
  lcd.setCursor(1, 1);
  lcd.print("Please wait....");
  delay(2000);
  lcd.clear();

  for (int i=0; i<4; i++) {
    lcd.noBacklight();
    delay(250);
    lcd.backlight();
    delay(250);
  }

  lcd.setCursor(3, 0);
  lcd.print("Water Tank");
  lcd.setCursor(3, 1);
  lcd.print("Controller");
  delay(3000);
  lcd.clear();
  
  lcd.setCursor(3, 0);
  lcd.print("Developed");
  lcd.setCursor(5, 1);
  lcd.print("By -");
  delay(2000);
  lcd.clear();

  lcd.setCursor(2, 0);
  lcd.print("Mr. Aye Chan");
  lcd.setCursor(3, 1);
  lcd.print("Aung Thwin");
  delay(2000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("ayechanaungthwin");
  lcd.setCursor(3, 1);
  lcd.print("@gmail.com");
  delay(2000);
  lcd.clear();

  for (int i=3; i>0; i--) {
    lcd.setCursor(2, 0);
    lcd.print("Starting in..");
    lcd.setCursor(7, 1);
    lcd.print(i);
    delay(1000);
  }

  //Show Scan
  showLCD("Scanning:", "Incoming signals", 4, 0);
}

int passCount = 0;

void loop() {
  
  //Scan Signal
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  boolean isPowerOff = true;
  String rcv="ACAT";
  if (driver.recv(buf, &buflen)) {
    isPowerOff=false;
    resetTransmission();
    for (int i=0; i<buflen; i++) {
      rcv += (char) buf[i];
    }
    lightUp(rcv);
  }

  if (rcv=="ACAT"&&isPowerOff) {
    passCount++;
    delay(300);
  }
  else {
    resetTransmission();
  }

  if (passCount==5) {
    resetTransmission();
    lightUp("powerOff");
  }
}

void resetTransmission() {
  passCount=0;
}

void errorSound() {
  for (int i=0; i<3; i++) {
    tone(buzzer, i*1000, 500);
    delay(500);
  }
}

void melodySound() {
  //int melody[] = {262, 196,196, 220, 196,0, 247, 262}; //Low
  //int melody[] = {524, 392, 392, 440, 392, 0, 494, 524}; //Medium
  int melody[] = {1048, 784, 784, 880, 784, 0, 988, 1048}; //High

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {4, 8, 8, 4,4,4,4,4 };

  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000/noteDurations[thisNote];
    tone(buzzer, melody[thisNote],noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(buzzer);
  }
}

void resetAllLeds() {
  resetOnSensorFailure();
  //digitalWrite(sensorFailure, LOW);
}

void resetOnSensorFailure() {
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(blue, LOW);
}

int blinkDelayTime=300;

void blink(int loc) {
  resetAllLeds();
  digitalWrite(loc, HIGH);
  delay(blinkDelayTime/2);
  digitalWrite(loc, LOW);
  delay(blinkDelayTime/2);
}

void errorBlink(int loc) {
  digitalWrite(loc, HIGH);
  delay(blinkDelayTime/2/3);
  digitalWrite(loc, LOW);
  delay(blinkDelayTime/2/3);
}

int count=0;
String repeatWord="begin";
String firstWord="begin";
String secondWord="begin";
  
String filter(String word, int frequency) {
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

void lightUp(String rcv) {
    boolean fillWater = false;
    Serial.print(rcv);
    //rcv = filter(rcv, 2); //filter 2 changes
    //Serial.println(rcv);
    
    if (rcv=="powerOff") {
      showLCD("Transmitter is", "switched off", 1, 2);
      melodySound();
      resetAllLeds();
      errorBlink(red);
      errorBlink(green);
      errorBlink(blue);
    }
    else if (rcv=="ACATsensorFailure") {
      showLCD("Sensor Failure", "in the tank!", 1, 2);
      errorSound();
      resetOnSensorFailure();
      errorBlink(red);
      errorBlink(green);
      errorBlink(blue);
      //digitalWrite(sensorFailure, HIGH); //OFF
    }
    else if (rcv=="ACATstopRed") {
      showLCD("Water tank is..", "almost empty", 1, 2);
      resetAllLeds();
      digitalWrite(red, HIGH);
    }
    else if (rcv=="ACATstopGreen") {
      showLCD("Full water", "in tank!", 3, 4);
      resetAllLeds();
      digitalWrite(green, HIGH);
    }
    else if (rcv=="ACATstopBlue") {
      showLCD("Half water is", "left in tank", 1, 2);
      resetAllLeds();
      digitalWrite(blue, HIGH);
    }
    
    else if (rcv=="ACATstartBlinkRed") {
      showLCD("Filling water:", "Approaching 1/3", 1, 0);
      fillWater = true;
      blink(red);
    }
    else if (rcv=="ACATstartBlinkGreen") {
      showLCD("Filling water:", "Approaching 3/3", 1, 0);
      fillWater = true;
      blink(green);
    }
    else if (rcv=="ACATstartBlinkBlue") {
      showLCD("Filling water:", "Approaching 2/3", 1, 0);
      fillWater = true;
      blink(blue);
    }
  
    if (fillWater) {
       digitalWrite(driveRelay, HIGH);
    }
    else {
      digitalWrite(driveRelay, LOW);
    }
}

String storeLastShownInLCD = "begin";

void showLCD(String first, String second, int cur1, int cur2) {
  if (second!=storeLastShownInLCD) {
    storeLastShownInLCD=second;
    lcd.clear();
    lcd.setCursor(cur1, 0);
    lcd.print(first);
    lcd.setCursor(cur2, 1);
    lcd.print(second);
  }
  if (first=="Sensor Failure"||first=="Transmitter is") {
    lcd.noBacklight();
    delay(250);
    lcd.backlight();
    delay(250);
  }
  if (first=="Filling water:") {
    
  }
}
