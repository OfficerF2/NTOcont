#include <Wire.h>
#include <VL53L0X.h>
VL53L0X lox;

void setup() {
  // Инициализация последовательного порта
  Serial.begin(115200);
  // Инициализация датчика
  Wire.begin();
  lox.init();
  lox.setTimeout(500);
  // параметры для режима высокой точности
  lox.setMeasurementTimingBudget(200000);
}

void loop() {
  float dist = lox.readRangeSingleMillimeters();
  Serial.println("Distance  = " + String(dist, 0) + " mm  ");
  if (dist <= 60){
    int value = 0;
    for(int i = 0; i<= 60; i++){
      value = i * 1.66666667;
      Serial.println("Зарядка:" + String(value) + "%");
      delay(1000);

    }
  }
  delay(250);
}