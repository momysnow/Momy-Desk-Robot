// MyWiFiManager.cpp
#include "MyWifiManager.h"

MyWifiManager::MyWifiManager(DisplayTools* display) {
  this->display = display;
  connected = false;
}

void MyWifiManager::begin(const char* ota_host_name) {
  display->writeThreeLinesOfText("Wifi", "Connection", "Start");
  
  WiFi.mode(WIFI_STA);
  wm.setConnectTimeout(30);
  wm.setAPCallback([this]() {
    configModeCallback();
  });

  uint32_t chipId = 0;
  for (int i = 0; i < 17; i = i + 8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  host_name = String(ota_host_name) + String(chipId);

  connected = wm.autoConnect(host_name.c_str());

  if (connected) {
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    ArduinoOTA.setHostname(host_name.c_str());
    ArduinoOTA.begin();

    display->writeThreeLinesOfText("Connected", "To", "Wi-Fi");
    display->writeThreeLinesOfText("Name for", "update", ota_host_name);
  }
}

void MyWifiManager::handle() {
  ArduinoOTA.handle();
}

bool MyWifiManager::isConnected() {
  return connected;
}

void MyWifiManager::configModeCallback() {
  Serial.println("Failed to connect");

  String apSSID = wm.getConfigPortalSSID();

  display->writeThreeLinesOfText("Failed", "to", "Connect");
  display->setupSmallText();
  display->writeThreeLinesOfText("Connect", "to", apSSID);

  unsigned long startTime = millis();

  while (WiFi.softAPgetStationNum() == 0 && (millis() - startTime < 60000)) {
    delay(100);
  }

  if (WiFi.softAPgetStationNum() == 0) {
    Serial.println("Nessun client connesso entro 60 secondi. Riavvio del dispositivo...");
    delay(1000);
    ESP.restart();
  }

  display->clear();
}