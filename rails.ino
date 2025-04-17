#include <Wire.h>
#include <VL53L0X.h>
VL53L0X lox;
#include <PCA9634.h>
#include <ESP32_Servo.h>
Servo myservo;
PCA9634 testModule(0x1C);
void setup() {

  Serial.begin(115200);
  myservo.attach(14);
  Wire.begin();
  lox.init();
  lox.setTimeout(500);

  lox.setMeasurementTimingBudget(200000);
    testModule.begin();
    for (int channel = 0; channel < testModule.channelCount(); channel++)
  {
    testModule.setLedDriverMode(channel, PCA9634_LEDOFF); // выключить все светодиоды в режиме 0/1
  }
}

void loop() {
    for (int channel = 0; channel < testModule.channelCount(); channel++)
  {
    testModule.setLedDriverMode(channel, PCA9634_LEDPWM); // установка режима ШИМ (0-255)
  }
  float dist = lox.readRangeSingleMillimeters();
  
     if (dist <= 60){
    testModule.write1(3, 0x90);
    testModule.write1(2, 0x00);
    testModule.write1(5, 0x00); 
      myservo.write(180);
     }else{
      myservo.write(110);
    testModule.write1(3, 0x00);
    testModule.write1(2, 0x00);
    testModule.write1(5, 0x00); 
    delay(500);
        testModule.write1(3, 0x90);
    testModule.write1(2, 0x00);
    testModule.write1(5, 0x00);
        delay(500);
        testModule.write1(3, 0x00);
    testModule.write1(2, 0x00);
    testModule.write1(5, 0x00);
     }





  Serial.println("Distance  = " + String(dist, 0) + " mm  ");
  delay(250);
}