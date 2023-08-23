#ifndef SERVER_H
#define SERVER_H

#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

extern ESP8266WebServer server;
extern HTTPClient http;

void serverSetup();

void handleLocalWifiConfig();

void getLocalWifiConfig();

void setLocalWifiConfig();

void setDefaultLocalWiFiConfig();

void handleExternalWifiConfig();

void getExternalWifiConfig();

void setExternalWifiConfig();

void getHumidityLevel();

void restartESP();

#endif
