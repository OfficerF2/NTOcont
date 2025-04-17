#include <Wire.h>
#include <VL53L0X.h>
#include "mcp3021.h"
#include <MGS_FR403.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "POCO M3";
const char* password = "007008009124";

WebServer server(80);

MGS_FR403 Fire;
float visib = 0;
byte ADDR = 0b011;
MCP3021 mcp3021;
VL53L0X lox;

const float air_value = 561.0;
const float water_value = 293.0;
String status = "Свободно";
String lastAlert = "Нет";

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
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
  
  Serial.print("IP адрес: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  server.handleClient();
  
  // Чтение датчиков
  Fire.get_ir_and_vis();
  visib = Fire.vis_data;
  float dist = lox.readRangeSingleMillimeters();
  float adc0 = mcp3021.readADC();
  float h = map(adc0, air_value, water_value, 0, 100);

  // Проверка событий
  if (h > 30) {
    lastAlert = "Протечка воды!";
    Serial.println(lastAlert);
  } 
  else if (visib >= 3500) {
    lastAlert = "Обнаружен пожар!";
    Serial.println(lastAlert);
  }

  // Логика зарядки
  if (dist <= 60 && !isCharging) {
    status = "Занято";
    isCharging = true;
    chargeStartTime = millis();
    initialCharge = random(0, 21);
  }

  if (isCharging) {
    unsigned long elapsed = millis() - chargeStartTime;
    int progress = initialCharge + ((elapsed * (100 - initialCharge)) / chargeDuration);
    if (progress >= 100) {
      isCharging = false;
      status = "Свободно";
    }
  }
  
  delay(250);
}

void handleRoot() {
  String html = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Умный город</title>
    <style>
        .dtach-container { display: flex; flex-wrap: wrap; gap: 15px; padding: 10px 0; }
        .dtach { border: 2px solid #4a90e2; border-radius: 10px; padding: 15px; width: 200px; }
        #alerts { list-style-type: none; padding-left: 0; }
    </style>
    <script>
        function updateData() {
            fetch('/data')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('distanceValue').textContent = data.distance;
                    document.getElementById('fireValue').textContent = data.fire;
                    document.getElementById('statusValue').textContent = data.status;
                    
                    if (data.alert && data.alert !== "Нет") {
                        const li = document.createElement('li');
                        li.textContent = data.alert;
                        document.getElementById('alerts').prepend(li);
                        
                        if (document.getElementById('alerts').children.length > 5) {
                            document.getElementById('alerts').lastChild.remove();
                        }
                    }
                    
                    setTimeout(updateData, 1000);
                })
                .catch(error => {
                    console.error('Ошибка:', error);
                    setTimeout(updateData, 5000);
                });
        }
        document.addEventListener('DOMContentLoaded', updateData);
    </script>
</head>
<body>
    <h1>Данные с датчиков</h1>
    <div class="dtach-container">
        <div class="dtach" id="dist">Расстояние: <span id="distanceValue">-</span> мм</div>
        <div class="dtach" id="Fire">Огонь: <span id="fireValue">-</span></div>
        <div class="dtach" id="Closed">Статус: <span id="statusValue">-</span></div>
        <div class="dtach" id="Allert">
            <h2>Оповещения:</h2>
            <ul id="alerts"></ul>
        </div>
    </div>
</body>
</html>
)=====";
  
  server.send(200, "text/html", html);
}

void handleData() {
  String json = "{";
  json += "\"distance\":" + String(lox.readRangeSingleMillimeters()) + ",";
  json += "\"fire\":" + String(visib) + ",";
  json += "\"status\":\"" + status + "\",";
  json += "\"alert\":\"" + lastAlert + "\"";
  json += "}";
  
  lastAlert = "Нет"; // Сбрасываем оповещение после отправки
  server.send(200, "application/json", json);
}