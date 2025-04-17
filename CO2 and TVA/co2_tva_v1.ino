#include "SparkFun_SGP30_Arduino_Library.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
Adafruit_BME280 TVASensor;
SGP30 CO2Sensor;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  if (CO2Sensor.begin() == false) {
    Serial.println("No SGP30 Detected. Check connections.");
    while (1);
  }
  CO2Sensor.initAirQuality();
  bool bme_status = TVASensor.begin();
  if (!bme_status) {
    Serial.println("Не найден по адресу 0х77, пробую другой...");
    bme_status = TVASensor.begin(0x76);
    if (!bme_status)
      Serial.println("Датчик не найден, проверьте соединение");
  }
}

void loop() {
  // первые 15 измерений будут неизменными
  delay(1000);
  float t = TVASensor.readTemperature();
  float h = TVASensor.readHumidity();
  float p = TVASensor.readPressure() / 100.0F;
  CO2Sensor.measureAirQuality();
  Serial.print("CO2: ");
  Serial.print(CO2Sensor.CO2);
  Serial.print(" ppm\tTVOC: ");
  Serial.print(CO2Sensor.TVOC);
  Serial.println(" ppb");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C\tHumidity: ");
  Serial.print(h);
  Serial.print(" %\tPressure: ");
  Serial.print(p);
  Serial.println(" hPa");
}