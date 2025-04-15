#include <BH1750.h>
#include <PCA9634.h>
#include <Wire.h>
/*Обнаружен модуль MGB-RGB3 по адресу: 0x08 !
Обнаружен модуль MGB-RGB3 по адресу: 0x0C !
Обнаружен модуль MGB-RGB3 по адресу: 0x18 !*/
BH1750 LightSensor_1; // экземпляры пяти датчиков освещенности
BH1750 LightSensor_2;
BH1750 LightSensor_3;
BH1750 LightSensor_4;
PCA9634 testModule(0x08);
PCA9634 testModule1(0x0C);
PCA9634 testModule2(0x18);
PCA9634 testModule3(0x1C);

#define I2C_HUB_ADDR        0x70
#define EN_MASK             0x08
#define DEF_CHANNEL         0x00
#define MAX_CHANNEL         0x08
/*
  I2C порт 0x07 - выводы GP16 (SDA), GP17 (SCL)
  I2C порт 0x06 - выводы GP4 (SDA), GP13 (SCL)
  I2C порт 0x05 - выводы GP14 (SDA), GP15 (SCL)
  I2C порт 0x04 - выводы GP5 (SDA), GP23 (SCL)
  I2C порт 0x03 - выводы GP18 (SDA), GP19 (SCL)
*/
void setup() {
  // Инициализация последовательного порта
  Serial.begin(115200);
  testModule.begin();
  testModule1.begin();
  testModule2.begin();
  testModule3.begin();
  Wire.begin();
  delay(1000);

  // Инициализация датчика #1
  setBusChannel(0x03); // функция смены I2C-порта
  LightSensor_1.begin();

  // Инициализация датчика #2
  setBusChannel(0x04); // функция смены I2C-порта
  LightSensor_2.begin();

  // Инициализация датчика #3
  setBusChannel(0x05); // функция смены I2C-порта
  LightSensor_3.begin();

  setBusChannel(0x07); // функция смены I2C-порта
  LightSensor_4.begin();

    for (int channel = 0; channel < testModule.channelCount(); channel++)
  {
    testModule.setLedDriverMode(channel, PCA9634_LEDON);
    Serial.println(channel);
    delay(500);
    testModule.setLedDriverMode(channel, PCA9634_LEDOFF);
    delay(500);
  }
  for (int channel = 0; channel < testModule.channelCount(); channel++)
  {
    testModule.setLedDriverMode(channel, PCA9634_LEDPWM); // установка режима ШИМ (0-255)
  }

    for (int channel = 0; channel < testModule1.channelCount(); channel++)
  {
    testModule1.setLedDriverMode(channel, PCA9634_LEDON);
    Serial.println(channel);
    delay(500);
    testModule1.setLedDriverMode(channel, PCA9634_LEDOFF);
    delay(500);
  }
  for (int channel = 0; channel < testModule1.channelCount(); channel++)
  {
    testModule1.setLedDriverMode(channel, PCA9634_LEDPWM); // установка режима ШИМ (0-255)
  }
    for (int channel = 0; channel < testModule2.channelCount(); channel++)
  {
    testModule2.setLedDriverMode(channel, PCA9634_LEDON);
    Serial.println(channel);
    delay(500);
    testModule2.setLedDriverMode(channel, PCA9634_LEDOFF);
    delay(500);
  }
  for (int channel = 0; channel < testModule2.channelCount(); channel++)
  {
    testModule2.setLedDriverMode(channel, PCA9634_LEDPWM); // установка режима ШИМ (0-255)
  }
    for (int channel = 0; channel < testModule3.channelCount(); channel++)
  {
    testModule3.setLedDriverMode(channel, PCA9634_LEDON);
    Serial.println(channel);
    delay(500);
    testModule3.setLedDriverMode(channel, PCA9634_LEDOFF);
    delay(500);
  }
  for (int channel = 0; channel < testModule3.channelCount(); channel++)
  {
    testModule3.setLedDriverMode(channel, PCA9634_LEDPWM); // установка режима ШИМ (0-255)
  }
}
void loop() {
  // Считывание датчиков
  setBusChannel(0x03);
  float light1 = LightSensor_1.readLightLevel();
  setBusChannel(0x04);
  float light2 = LightSensor_2.readLightLevel();
  setBusChannel(0x05);
  float light3 = LightSensor_3.readLightLevel();
  setBusChannel(0x07);
  float light4 = LightSensor_4.readLightLevel();


  if (light1 < 250){
  testModule.write1(3, 0x90);
  testModule.write1(2, 0x90);
  testModule.write1(5, 0x00);
  }else{
  testModule.write1(3, 0x00);
  testModule.write1(2, 0x00);
  testModule.write1(5, 0x00);
  }
  if (light2 < 250){
  testModule1.write1(3, 0x90);
  testModule1.write1(2, 0x90);
  testModule1.write1(5, 0x00);
  }else{
  testModule1.write1(3, 0x00);
  testModule1.write1(2, 0x00);
  testModule1.write1(5, 0x00);
  }
  if (light3 < 250){
  testModule2.write1(3, 0x90);
  testModule2.write1(2, 0x90);
  testModule2.write1(5, 0x00);
  }  else{
  testModule2.write1(3, 0x00);
  testModule2.write1(2, 0x00);
  testModule2.write1(5, 0x00);
  }
  if (light4 < 250){
  testModule3.write1(3, 0x90);
  testModule3.write1(2, 0x90);
  testModule3.write1(5, 0x00);
  } else{
  testModule3.write1(3, 0x00);
  testModule3.write1(2, 0x00);
  testModule3.write1(5, 0x00);
  }
  Serial.println("-------------------------------------------------------");
  Serial.println("Ambient light intensity 1: " + String(light1, 1) + " lx");
  Serial.println("Ambient light intensity 2: " + String(light2, 1) + " lx");
  Serial.println("Ambient light intensity 3: " + String(light3, 1) + " lx");
  Serial.println("Ambient light intensity 4: " + String(light3, 1) + " lx");
  delay(1000);
}

bool setBusChannel(uint8_t i2c_channel)
{
  if (i2c_channel >= MAX_CHANNEL)
  {
    return false;
  }
  else
  {
    Wire.beginTransmission(I2C_HUB_ADDR);
    Wire.write(i2c_channel | EN_MASK); // для микросхемы PCA9547
	// Wire.write(0x01 << i2c_channel); // Для микросхемы PW548A
    Wire.endTransmission();
    return true;
  }
}