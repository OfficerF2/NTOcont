#include <Wire.h>
#include "mcp3021.h"
byte ADDR = 0b011;


MCP3021 mcp3021;

const float air_value = 561.0;
const float water_value = 293.0;
const float moisture_0 = 0.0;
const float moisture_100 = 100.0;

void setup() {

  Serial.begin(115200);
  Wire.begin();
  mcp3021.begin(ADDR);
}

void loop() {
  float adc0 = mcp3021.readADC();
  float h = map(adc0, air_value, water_value, moisture_0, moisture_100);
  //Serial.println("Water level = " + String(h, 1) + " %");
  if(h>30){
    Serial.println("Протечка!");
  }
  delay(250);
}