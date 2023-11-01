#include "boot/library/WifiManager.h"
#include <TensorFlowLite.h>

WifiManager wifiManager;

TaskHandle_t Scan_api;

TaskHandle_t loop;

#include "boot/src/logo.h"             // Sketch tab header for xbm images

#include <TFT_eSPI.h>        // Hardware-specific library

TFT_eSPI tft = TFT_eSPI();   // Invoke library

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

  tft.begin();
  tft.fillScreen(TFT_BLACK); // Black screen fill

  int x = (240-logoWidth)/2;
  int y = (240-logoHeight)/2 - 10; // - offset

  // Draw bitmap with top left corner at x,y with foreground only color
  // Bits set to 1 plot as the defined color, bits set to 0 are not plotted
  //              x  y  xbm   xbm width  xbm height  color
  tft.drawXBitmap(x, y, logo, logoWidth, logoHeight, TFT_WHITE);

  if (!wifiManager.connect_wifi("ssid", "password")) {
    Serial.println("Failed to connect to Wi-Fi, starting access point");
    wifiManager.start_ap();
    wifiManager.create_web_server();
  } else {
    Serial.println("Connected to Wi-Fi");
    wifiManager.create_web_server();
  }

  delay(500);
  tft.fillScreen(TFT_BLACK); // Black screen fill
}

void loop() {
  //detect wake word with tensorflow lite

}