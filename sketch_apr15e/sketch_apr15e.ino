#include <BH1750.h>
#include <Wire.h>

BH1750 LightSensor_1; // экземпляры пяти датчиков освещенности
BH1750 LightSensor_2;
BH1750 LightSensor_3;
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

  // Инициализация I2C
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
}
void loop() {
  // Считывание датчиков
  setBusChannel(0x03);
  float light1 = LightSensor_1.readLightLevel();
  setBusChannel(0x04);
  float light2 = LightSensor_2.readLightLevel();
  setBusChannel(0x05);
  float light3 = LightSensor_3.readLightLevel();
  Serial.println("-------------------------------------------------------");
  Serial.println("Ambient light intensity 1: " + String(light1, 1) + " lx");
  Serial.println("Ambient light intensity 2: " + String(light2, 1) + " lx");
  Serial.println("Ambient light intensity 3: " + String(light3, 1) + " lx");
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