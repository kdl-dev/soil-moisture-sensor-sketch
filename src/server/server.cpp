#include "server.h"

#include <Arduino_JSON.h>
#include <LittleFS.h>

#include "../macro.h"
#include "../utils/utils.h"

HTTPClient http;
ESP8266WebServer server(80);

void serverSetup() {
  server.on("/config/wifi/external", handleExternalWifiConfig);
  server.on("/config/wifi/local", handleLocalWifiConfig);
  server.on("/config/wifi/local/setDefault", setDefaultLocalWiFiConfig);
  server.on("/restart", restartESP);

  server.begin();
}

void handleLocalWifiConfig() {
  switch (server.method()) {
    case HTTPMethod::HTTP_POST:
      setLocalWifiConfig();
      return;
    case HTTPMethod::HTTP_GET:
      getLocalWifiConfig();
      return;
defualt:
      server.send(405);
      return;
  }
}

void getLocalWifiConfig() {
  File configFile = LittleFS.open(LOCAL_WIFI_CONFIG, "r");
  String fileContent = configFile.readString();
  configFile.close();

  JSONVar json;
  json = json.parse(fileContent);
  server.send(200, "application/json", JSON.stringify(json));
}

void setLocalWifiConfig() {
  JSONVar json;
  json = json.parse(server.arg("plain"));

  if (!json.hasOwnProperty("ssid")) {
    server.send(400, "application/json", "{\"err\":\"missing 'ssid' parameter\"}");
    return;
  }

  if (!json.hasOwnProperty("pass")) {
    server.send(400, "application/json", "{\"err\":\"missing 'pass' parameter\"}");
    return;
  }

 String err = validateSSID(json["ssid"]);
  if (err != "") {
    server.send(400, "application/json", "{\"err\": " + err + "}");
    return;
  }

  err = validatePass(json["pass"]);
  if (err != "") {
    server.send(400, "application/json", "{\"err\": " + err + "}");
    return;
  }

  File configFile = LittleFS.open(LOCAL_WIFI_CONFIG, "w");
  configFile.print(JSON.stringify(json));
  configFile.close();

  server.send(200);
}

void setDefaultLocalWiFiConfig() {
  if (server.method() != HTTPMethod::HTTP_POST) {
    server.send(405);
    return;
  }

  File configFile = LittleFS.open(LOCAL_WIFI_CONFIG, "w");

  JSONVar json;
  json["ssid"] = DEFAULT_LOCAL_SSID;
  json["pass"] = DEFAULT_LOCAL_PASS;

  configFile.print(JSON.stringify(json));
  configFile.close();

  server.send(200);
}

void handleExternalWifiConfig() {
  switch (server.method()) {
    case HTTPMethod::HTTP_POST:
      setExternalWifiConfig();
      return;
    case HTTPMethod::HTTP_GET:
      getExternalWifiConfig();
      return;
defualt:
      server.send(405);
      return;
  }
}

void getExternalWifiConfig() {
  File configFile = LittleFS.open(EXTERNAL_WIFI_CONFIG, "r");
  String fileContent = configFile.readString();
  configFile.close();

  JSONVar json;
  json = json.parse(fileContent);
  server.send(200, "application/json", JSON.stringify(json));
}

void setExternalWifiConfig() {
  JSONVar json;
  json = json.parse(server.arg("plain"));

  if (!json.hasOwnProperty("ssid")) {
    server.send(400, "application/json", "{\"err\":\"missing 'ssid' parameter\"}");
    return;
  }

  if (!json.hasOwnProperty("pass")) {
    server.send(400, "application/json", "{\"err\":\"missing 'pass' parameter\"}");
    return;
  }

  String err = validateSSID(json["ssid"]);
  if (err != "") {
    server.send(400, "application/json", "{\"err\": " + err + "}");
    return;
  }

  err = validatePass(json["pass"]);
  if (err != "") {
    server.send(400, "application/json", "{\"err\": " + err + "}");
    return;
  }

  File configFile = LittleFS.open(EXTERNAL_WIFI_CONFIG, "w");
  configFile.print(JSON.stringify(json));
  configFile.close();

  server.send(200);
}

void restartESP() {
  server.send(200);
  delay(1000);
  ESP.restart();
} 
