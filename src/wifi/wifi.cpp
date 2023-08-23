#include "wifi.h"
#include "../macro.h"

#include <ESP8266WiFi.h>
#include <Arduino_JSON.h>
#include <LittleFS.h>

IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

void wifiSetup() {
  wifiAPSetup();
  wifiSTASetup();
}

void wifiAPSetup() {
  File configFile = LittleFS.open(LOCAL_WIFI_CONFIG, "r");

  String fileContent = configFile.readString();
  JSONVar json;

  json = json.parse(fileContent);

  String localSSID = json["ssid"];
  String localPass = json["pass"];

  Serial.printf("Local SSID %s\n", localSSID);
  Serial.printf("Local Pass %s\n", localPass);

  bool isSuccess = WiFi.softAP(localSSID, localPass);

  Serial.printf("WiFi start with user config: %d\n", isSuccess);
  WiFi.softAPConfig(local_ip, gateway, subnet);

  configFile.close();
}

void wifiSTASetup() {
  File configFile = LittleFS.open(EXTERNAL_WIFI_CONFIG, "r");

  String fileContent = configFile.readString();
  JSONVar json;

  json = json.parse(fileContent);

  String externalSSID = json["ssid"];
  String externalPass = json["pass"];

  bool withoutConnection = true;
  WiFi.begin(externalSSID, externalPass);
  for (int i = 0; i < 10; i++) {
    if (WiFi.status() != WL_CONNECTED) {
      delay(1000);
    } else {
      withoutConnection = false;
      break;
    }
  }

  if (withoutConnection) {
    WiFi.disconnect(true);
    Serial.println("Start without connecting");
    wifiAPSetup();
    WiFi.begin();
  }

  configFile.close();
}

