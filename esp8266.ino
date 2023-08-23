#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>
#include <LittleFS.h>

#include "src/macro.h"
#include "src/wifi/wifi.h"
#include "src/server/server.h"
#include "src/utils/utils.h"

WiFiClient client;
JSONVar json;

void setup() {
  Serial.begin(9600);

  pinMode(pin, INPUT);

  if (!LittleFS.begin()) {
    Serial.println("LittleFS Mount failed");
    return;
  }

  wifiSetup();
  serverSetup();
}

void loop() {
  server.handleClient();

  json["humidity_sensor_id"] = 123;
  json["humidity_percent"] = getSoilMoisturePercent();
  
  sendDataToServer(json.stringify(json));

  delay(1000);
}

int getSoilMoisturePercent() {
  int soilMoistureValue = analogRead(pin);
  int soilmoisturepercent = map(soilMoistureValue, AirMinValue, WaterMAXValue, 0, 100) - 50;  // адаптируем значения от 0 до 100,

  if (soilmoisturepercent > 100) {
    soilmoisturepercent = 100;
  } else if (soilmoisturepercent < 0) {
    soilmoisturepercent = 0;
  }

  return soilmoisturepercent;
}

void sendDataToServer(String data) {
  http.begin(client, SERVER_ADDRESS);
  http.addHeader("Content-type", "application/json");
  http.POST(data);
}
