#include <FastLED.h>
#include <Wire.h>
#define LED_PIN     15
#define LED_PIN2    14
#define NUM_LEDS    3
#define BRIGHTNESS  50
#include <VL53L0X.h>
VL53L0X lox;
CRGB leds[NUM_LEDS];

// Переменные для подсчета машин
int count = 0;
unsigned long lastResetTime = 0;
const unsigned long resetInterval = 60000;
unsigned long lastDetectionTime = 0;
const unsigned long carCooldown = 0;

// Состояние светофора
enum TrafficLightState { RED, YELLOW_AFTER_RED, GREEN, YELLOW_AFTER_GREEN };
TrafficLightState currentState = RED;
unsigned long stateStartTime;

// Интервалы светофора
unsigned long redDuration = 15000;
unsigned long yellowDuration = 2000;   
unsigned long greenDuration = 15000;  
unsigned long highTrafficRedDuration = 5000;    
unsigned long highTrafficGreenDuration = 20000; 

void setup() {
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, LED_PIN2>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  Wire.begin();
  lox.init();
  lox.setTimeout(500);
  lox.setMeasurementTimingBudget(200000);
  Serial.begin(115200);
  lastResetTime = millis();
  stateStartTime = millis();
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastDetectionTime > carCooldown) {
    float dist = lox.readRangeSingleMillimeters();
    if (dist < 60) {
      count++;
      lastDetectionTime = currentTime;
      Serial.print("Машина обнаружена! Всего: ");
      Serial.println(count);

      delay(100);
    }
  }

  if (currentTime - lastResetTime >= resetInterval) {
    Serial.print("Сброс счетчика. Было машин: ");
    Serial.println(count);
    count = 0;
    lastResetTime = currentTime;
  }


  bool highTraffic = (count >= 4);
  unsigned long currentRedDuration = highTraffic ? highTrafficRedDuration : redDuration;
  unsigned long currentGreenDuration = highTraffic ? highTrafficGreenDuration : greenDuration;

  // 4. Управление светофором
  switch(currentState) {
    case RED:
      leds[0] = CRGB::Red;
      leds[1] = CRGB::Black;
      leds[2] = CRGB::Black;
      if (currentTime - stateStartTime >= currentRedDuration) {
        currentState = YELLOW_AFTER_RED;
        stateStartTime = currentTime;
      }
      break;
      
    case YELLOW_AFTER_RED:
      leds[0] = CRGB::Black;
      leds[1] = CRGB::Yellow;
      leds[2] = CRGB::Black;
      if (currentTime - stateStartTime >= yellowDuration) {
        currentState = GREEN;
        stateStartTime = currentTime;
      }
      break;
      
    case GREEN:
      leds[0] = CRGB::Black;
      leds[1] = CRGB::Black;
      leds[2] = CRGB::Green;
      if (currentTime - stateStartTime >= currentGreenDuration) {
        currentState = YELLOW_AFTER_GREEN;
        stateStartTime = currentTime;
      }
      break;
      
    case YELLOW_AFTER_GREEN:
      leds[0] = CRGB::Black;
      leds[1] = CRGB::Yellow;
      leds[2] = CRGB::Black;
      if (currentTime - stateStartTime >= yellowDuration) {
        currentState = RED;
        stateStartTime = currentTime;
      }
      break;
  }

  FastLED.show();
  delay(10);
}