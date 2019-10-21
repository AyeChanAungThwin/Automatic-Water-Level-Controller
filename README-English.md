# Automatic-Water-Tank-Controller
## Details
> This is an arduino project.
> This is useful when your switch and water tank is too far away to connect cable.
> If following links are failed, you can find it all(Video is not included.) in folder named "Required-Libruaries-And-Project."
> If you have any POSITIVE suggestion on this code, please let me know. I'll be glad to hear that.

## Explanation
- To transmit a text to the receiver circuit, the transmitter circuit will check whether different text is generating according to sensor. If the same text is receiving for 10 times, it changes the text that is happening for 10 times and make the same loop again and again. 
- This kind of filtration is required when the sensor is getting neither ~~OPEN~~ nor ~~CLOSE~~ because of water waves in the tank and we have to make sure it is ***HIGH*** or ***LOW***. And this makes this circuit more stable.
```
Real Time Happening		Filter Result
111				111
011				111
111				111
011				111
011				111
011				111
011				111
011				111
011				111
011				111
011				111
011				111
011				111
011				011
011				011
010				011
```
- At the text filtration method, I forced the receiver circuit to switch off when all sensor is getting HIGH regardless of the filter. This is required, too. This isn't about only about C program anymore. This is also about hardware. We're giving 3 analog signals to A0, A1 and A2 from a 5V pin and they will not be ***LOW*** unless grounded with 2k resistors to each analog pins. That's why when the sensor is ***CLOSED***, not only it gives 5 volts to analog pins but also goes into grounds. The lost in voltage changes both HIGH and LOW while filtration 10 times and never match the same text 10 times and never stop filling water. Trust me, that has happened to it. If you use (3x) 5k resistors, it won't be necessary. Just use 5k if you have it. Even if you use 5k, you don't need to change the code. Don't remove that code because it will make sure the switch to turn off.
- At the receiver Circuit, it will give you an alarm when the transmitter is off, sensor failure, open or close the switch and its LCD will show us the situation happening inside water tank.

## 433Mhz Transceiver
###### **(Video)** *13 mins* ######
- [Setting up wireless RF (433Mhz) Transceiver Module](https://www.youtube.com/watch?v=txSrx5druXg)
###### **(Code - already included in an IDE)**  ######
- Examples/RadioHead/ask/ask_transmitter
- Examples/RadioHead/ask/ask_receiver
###### **(Libruary)** ######
- [RadioHead Libruary](http://www.airspayce.com/mikem/arduino/RadioHead/index.html)

## I2C Adaptor
###### **(Video)** *46mins* ######
- [LCD Displays with Arduino](https://www.youtube.com/watch?v=wEbGhYjn4QI&t=1746s)
###### **(Code)** ######
- [I2C Address Scanner](https://playground.arduino.cc/Main/I2cScanner/)

## 16x2 LCD Display
###### **(Video)** *13mins* ######
- [LCD1602 with I2C module](https://www.youtube.com/watch?v=q9YC_GVHy5A)
###### **(Code - Scrolling text in one line)** ######
- [Marquee text in LCD display](https://forum.arduino.cc/index.php?topic=422542.0)
###### **(Libruary)** ######
- [LiquidCrystal-I2C-Libruary](https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library)
