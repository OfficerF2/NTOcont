#include <Wire.h>
#include <VL53L0X.h>
#include "mcp3021.h"
#include <MGS_FR403.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// Настройки WiFi и UDP
const char* ssid = "POCO M3";
const char* password = "007008009124";
const char* serverIP = "192.168.43.193";  // IP сервера (ESP32-CAM)
const int serverPort = 8888;             // Порт сервера
WiFiUDP udp;
MGS_FR403 Fire;
float visib = 0;
byte ADDR = 0b011;
MCP3021 mcp3021;
VL53L0X lox;

const float air_value = 561.0;
const float water_value = 293.0;
const float moisture_0 = 0.0;
const float moisature_100 = 100.0;
String status = "Свободно";

bool isCharging = false;
unsigned long chargeStartTime = 0;
const int chargeDuration = 60000;
int initialCharge = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Fire.begin();
  mcp3021.begin(ADDR);
  lox.init();
  lox.setTimeout(500);
  lox.setMeasurementTimingBudget(200000);
  randomSeed(analogRead(0));
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
}

void loop() {
  Fire.get_ir_and_vis();
  visib = Fire.vis_data;
  float dist = lox.readRangeSingleMillimeters();
  float adc0 = mcp3021.readADC();
  float h = map(adc0, air_value, water_value, 0, 100);
  if (h > 30) {
    sendAlert("WATER");
    Serial.println("Протечка!");
    delay(3000);
  }
  else if (visib >= 3500) {
    sendAlert("FIRE");
    Serial.println("Пожар");
    delay(3000);
  }else{
    sendAlert("NOTHING");
  }

  if (dist <= 60 && !isCharging) {
    status = "Занято";
    isCharging = true;
    chargeStartTime = millis();
    initialCharge = random(0, 21);
    Serial.println("Начало зарядки с " + String(initialCharge) + "%");
  }

  if (isCharging) {
    unsigned long elapsed = millis() - chargeStartTime;
    int progress = initialCharge + ((elapsed * (100 - initialCharge)) / chargeDuration);

    if (progress > 100) progress = 100;
    
    Serial.println("Зарядка: " + String(progress) + "%");
    
    if (progress >= 100) {
      isCharging = false;
      status = "Свободно";
      Serial.println("Зарядка завершена");
    }
  }

  delay(250);
}

void sendAlert(const char* alertType) {
  udp.beginPacket(serverIP, serverPort);
  udp.write((const uint8_t*)alertType, strlen(alertType));
  udp.endPacket();
  Serial.println("Sent: " + String(alertType));
}