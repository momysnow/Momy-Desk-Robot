// MyWiFiManager.h
#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <WiFiManager.h>
#include <ArduinoOTA.h>
#include "DisplayTools.h"

class MyWifiManager {
public:
  MyWifiManager(DisplayTools* display);
  void begin(const char* ota_host_name);
  void handle();
  bool isConnected();

private:
  WiFiManager wm;
  String host_name;
  bool connected;
  DisplayTools* display;

  void configModeCallback();
};

#endif