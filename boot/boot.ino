#include "WifiManager.h"

WifiManager wifiManager;

TaskHandle_t Scan_api;

TaskHandle_t loop;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(115200);
  delay(1000);

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
    wifiManager.handleClient(),   /* Task function. */
    "Scan_api",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Scan_api,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
    loop(),   /* Task function. */
    "loop",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    0,           /* priority of the task */
    &loop,      /* Task handle to keep track of created task */
    1);          /* pin task to core 1 */                  
  delay(500); 

  if (!wifiManager.connect_wifi("ssid", "password")) {
    Serial.println("Failed to connect to Wi-Fi, starting access point");
    wifiManager.start_ap();
    wifiManager.create_web_server();
  } else {
    Serial.println("Connected to Wi-Fi");
    wifiManager.create_web_server();
  }
}

void loop() {
  
}