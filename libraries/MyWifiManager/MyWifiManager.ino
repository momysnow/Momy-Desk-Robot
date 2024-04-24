#include <Arduino.h>
#include "MyWifiManager.h"
#include "DisplayTools.h"

// Creazione di un oggetto DisplayTools (sostituire con la tua implementazione)
DisplayTools display;

// Creazione di un oggetto MyWifiManager
MyWifiManager wifiManager(&display);

void setup() {
  Serial.begin(115200);

  // Inizializzazione del display
  display.init();

  // Inizializzazione del WiFi Manager
  wifiManager.begin("MyESPDevice");
}

void loop() {
  // Gestione delle connessioni WiFi
  wifiManager.handle();

  // Verifica se la connessione WiFi è attiva
  if (wifiManager.isConnected()) {
    Serial.println("WiFi connected");
  } else {
    Serial.println("WiFi not connected");
  }

  delay(1000);
}