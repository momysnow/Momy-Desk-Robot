#include "MqttHandler.h"

MqttHandler::MqttHandler() : mqttClient(espClient) {}

void MqttHandler::begin(const char* mqtt_server) {
  this->mqtt_server = mqtt_server;

  // Setup callback for WiFiManager
  wifiManager.setSaveConfigCallback(std::bind(&MqttHandler::saveConfigCallback, this));
  
  // Setup MQTT server
  mqttClient.setServer(mqtt_server, 1883);
  
  // Setup WiFi
  setupWiFi();
}

void MqttHandler::loop() {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
}

void MqttHandler::publish(const char* topic, const char* payload) {
  mqttClient.publish(topic, payload);
}

void MqttHandler::setCallback(MQTT_CALLBACK_SIGNATURE) {
  mqttClient.setCallback(callback);
}

void MqttHandler::setupWiFi() {
  if (!wifiManager.autoConnect("ESP32AP")) {
    Serial.println("Failed to connect and hit timeout");
    delay(3000);
    ESP.restart();
    delay(5000);
  }
}

void MqttHandler::reconnect() {
  if (millis() - lastReconnectAttempt > 5000) {
    if (wifiManager.getWiFiIsSaved()) {
      if (wifiManager.autoConnect()) {
        shouldSaveConfig = true;
      }
    } else {
      setupWiFi();
    }
    lastReconnectAttempt = millis();
  }
}

void MqttHandler::saveConfigCallback() {
  shouldSaveConfig = true;
}
