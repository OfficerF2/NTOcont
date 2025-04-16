#include <WiFi.h>
#include <WebServer.h>
#include <WiFiUdp.h>

const char* ssid = "POCO M3";
const char* password = "007008009124";
WebServer server(80);
WiFiUDP udp;
const int udpPort = 8888; // Должен совпадать с портом отправителя

// Структура для хранения данных
struct {
  float lightLevel = 0;
  String lastUpdate = "Нет данных";
} sensorData;

void setup() {
  Serial.begin(115200);
  
  // Подключение к WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nПодключено! IP: " + WiFi.localIP().toString());

  // Инициализация UDP
  if (udp.begin(udpPort)) {
    Serial.println("UDP сервер запущен на порту " + String(udpPort));
  } else {
    Serial.println("Ошибка инициализации UDP!");
  }

  // Маршруты веб-сервера
  server.on("/", HTTP_GET, handleRoot);
  server.on("/data", HTTP_GET, handleDataRequest);
  server.begin();
}

void loop() {
  server.handleClient();
  receiveUDPData();
}

void receiveUDPData() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    char packetBuffer[50];
    int len = udp.read(packetBuffer, sizeof(packetBuffer) - 1);
    if (len > 0) {
      packetBuffer[len] = '\0';
      String message = String(packetBuffer);
      Serial.println("Получено: " + message);

      // Парсинг сообщения
      int separatorIndex = message.indexOf(';');
      if (separatorIndex != -1) {
        String dataType = message.substring(0, separatorIndex);
        String valueStr = message.substring(separatorIndex + 1);
        
        if (dataType == "lux") {
          sensorData.lightLevel = valueStr.toFloat();
          sensorData.lastUpdate = getCurrentTime();
          Serial.println("Освещенность обновлена: " + valueStr + " лк");
        }
      }
    }
  }
}

String getCurrentTime() {
  return String(millis() / 1000) + " сек назад";
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
  html += "<meta http-equiv='refresh' content='2'>";
  html += "<title>Датчик освещенности</title></head><body>";
  html += "<h1>Текущая освещенность</h1>";
  html += "<p><strong>" + String(sensorData.lightLevel, 1) + " лк</strong></p>";
  html += "<p>Обновлено: " + sensorData.lastUpdate + "</p>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

void handleDataRequest() {
  String json = "{";
  json += "\"light\":" + String(sensorData.lightLevel, 1) + ",";
  json += "\"time\":\"" + sensorData.lastUpdate + "\"";
  json += "}";
  
  server.send(200, "application/json", json);
}