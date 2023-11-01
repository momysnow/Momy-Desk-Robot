#ifndef WifiManager_h
#define WifiManager_h

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

class WifiManager {
  public:
    WifiManager();
    bool connect_wifi(const char* ssid, const char* password);
    void scan_wifi();
    void start_ap();
    void load_known_networks();
    void add_known_network(const char* ssid, const char* password);
    void create_web_server();
    void handle_root();
    void handle_led_on();
    void handle_led_off();
    void handle_connect_wifi();
    void handleClient();
  private:
    const char* known_networks_path = "boot/db/known_networks.json";
    struct KnownNetwork {
      char ssid[32];
      char password[64];
    };
    KnownNetwork known_networks[10];
    int num_known_networks = 0;
    WebServer server;
    int led_pin = 2;
};

#endif
