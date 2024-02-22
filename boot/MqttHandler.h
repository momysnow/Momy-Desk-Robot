#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <Arduino.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <SimpleTimer.h>

class MqttHandler {
  public:
    MqttHandler();
    void begin(const char* mqtt_server);
    void loop();
    void publish(const char* topic, const char* payload);
    void setCallback(MQTT_CALLBACK_SIGNATURE);
    
  private:
    WiFiManager wifiManager;
    WiFiClient espClient;
    PubSubClient mqttClient;
    SimpleTimer timer;

    const char* mqtt_server;
    long lastReconnectAttempt = 0;
    bool shouldSaveConfig = false;
    
    void setupWiFi();
    void reconnect();
    void saveConfigCallback();
};

#endif
