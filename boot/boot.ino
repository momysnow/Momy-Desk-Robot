#include <TFT_eSPI.h>  // Hardware-specific library
#include <DHT.h>  // Include the DHT library
#include "sd_read_write.h"
#include "SD_MMC.h"

#include <Arduino.h>
#include <WiFi.h>
#include <EEPROM.h>
#include <WebServer.h>

TaskHandle_t Loop;
TaskHandle_t Client;

WebServer server(80);

// Dichiarazione delle reti Wi-Fi conosciute
const char *knownNetworks[][2] = {
  { "SSID1", "Password1" },
  { "SSID2", "Password2" }
  // Aggiungi altre reti conosciute qui
};
int numKnownNetworks = sizeof(knownNetworks) / sizeof(knownNetworks[0]);

#define SD_MMC_CMD 38 //Please do not modify it.
#define SD_MMC_CLK 39 //Please do not modify it. 
#define SD_MMC_D0  40 //Please do not modify it.

DHT dht(11, DHT11);
float temp = 0;
float humidity = 0;

TFT_eSPI tft = TFT_eSPI();             // Invoke custom library
TFT_eSprite eyes = TFT_eSprite(&tft);  // Invoke custom library
const unsigned int sreenW = 240;
const unsigned int sreenH = 240;

#include "Logo.h"
int w_eyes = 40;
int h_eyes = 70;
int close_h_eyes = 10;
int eyes_distance = 30;
int x_eyeL = (sreenW - eyes_distance * 2 - w_eyes) / 2;
int y_eyeL = 30 + (sreenH - h_eyes) / 2;
int x_eyeR = sreenW - ((sreenW - eyes_distance * 2 - w_eyes) / 2);
int y_eyeR = 30 + (sreenH - h_eyes) / 2;

void close_eyes() {

  // avvio animazione
  eyes.fillEllipse(x_eyeL, y_eyeL, w_eyes, h_eyes, TFT_WHITE);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(20);
  eyes.fillEllipse(x_eyeL, y_eyeL, w_eyes, h_eyes, TFT_BLACK);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes, TFT_BLACK);

  eyes.fillEllipse(x_eyeL, y_eyeL, w_eyes, h_eyes - 5, TFT_WHITE);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 5, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(20);
  eyes.fillEllipse(x_eyeL, y_eyeL, w_eyes, h_eyes - 5, TFT_BLACK);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 5, TFT_BLACK);

  eyes.fillEllipse(x_eyeL, y_eyeL, w_eyes, h_eyes - 15, TFT_WHITE);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 15, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(20);
  eyes.fillEllipse(x_eyeL, y_eyeL, w_eyes, h_eyes - 15, TFT_BLACK);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 15, TFT_BLACK);

  eyes.fillEllipse(x_eyeL, y_eyeL, w_eyes, h_eyes - 40, TFT_WHITE);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 40, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(20);
  eyes.fillEllipse(x_eyeL, y_eyeL, w_eyes, h_eyes - 40, TFT_BLACK);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 40, TFT_BLACK);

  // animazione centrale
  // 0.2 sec
  // rettangolo
  eyes.fillRect(x_eyeL-30, y_eyeL, w_eyes+30, close_h_eyes, TFT_WHITE);
  eyes.fillRect(x_eyeR-30, y_eyeR, w_eyes+30, close_h_eyes, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(350);
  eyes.fillRect(x_eyeL-30, y_eyeL, w_eyes+30, close_h_eyes, TFT_BLACK);
  eyes.fillRect(x_eyeR-30, y_eyeR, w_eyes+30, close_h_eyes, TFT_BLACK);

  // conclusione animazione   
  eyes.fillEllipse(x_eyeL, y_eyeL, w_eyes, h_eyes - 40, TFT_WHITE);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 40, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(20);
  eyes.fillEllipse(x_eyeL, y_eyeL, w_eyes, h_eyes - 40, TFT_BLACK);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 40, TFT_BLACK);

  eyes.fillEllipse(x_eyeL, y_eyeL, w_eyes, h_eyes - 15, TFT_WHITE);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 15, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(20);
  eyes.fillEllipse(x_eyeL, y_eyeL, w_eyes, h_eyes - 15, TFT_BLACK);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 15, TFT_BLACK);
  
  eyes.fillEllipse(x_eyeL, y_eyeL, w_eyes, h_eyes - 5, TFT_WHITE);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 5, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(20);
  eyes.fillEllipse(x_eyeL, y_eyeL, w_eyes, h_eyes - 5, TFT_BLACK);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 5, TFT_BLACK);

  eyes.fillEllipse(x_eyeL, y_eyeL, w_eyes, h_eyes, TFT_WHITE);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(20);
  eyes.fillEllipse(x_eyeL, y_eyeL, w_eyes, h_eyes, TFT_BLACK);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes, TFT_BLACK);
}

void wink_eyes() {

  // avvio animazione
  eyes.fillEllipse(x_eyeL, y_eyeL, w_eyes, h_eyes, TFT_WHITE);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(20);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes, TFT_BLACK);

  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 5, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(20);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 5, TFT_BLACK);

  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 15, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(20);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 15, TFT_BLACK);

  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 40, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(20);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 40, TFT_BLACK);

  // animazione centrale
  // 0.2 sec
  // rettangolo
  eyes.fillRect(x_eyeR-30, y_eyeR, w_eyes+30, close_h_eyes, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(350);
  eyes.fillRect(x_eyeR-30, y_eyeR, w_eyes+30, close_h_eyes, TFT_BLACK);

  // conclusione animazione
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 40, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(20);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 40, TFT_BLACK);

  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 15, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(20);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 15, TFT_BLACK);
  
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 5, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(20);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 5, TFT_BLACK);

  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(20);
  eyes.fillEllipse(x_eyeL, y_eyeL, w_eyes, h_eyes, TFT_BLACK);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes, TFT_BLACK);
}

void sleep_eyes() {
  // animazione centrale
  // 0.2 sec
  // rettangolo
  eyes.fillRect(x_eyeR-30, y_eyeR, w_eyes+30, close_h_eyes, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(350);
  eyes.fillRect(x_eyeR-30, y_eyeR, w_eyes+30, close_h_eyes, TFT_BLACK);
}

void setup() {
  dht.begin();
  Serial.begin(115200);
  
  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
    handleClientTask, /* Task function. */
    "Client", /* name of task. */
    10000,  /* Stack size of task */
    NULL, /* parameter of the task */
    1,  /* priority of the task */
    &Client,  /* Task handle to keep track of created task */
    0 /* pin task to core 0 */ 
  );

  SD_MMC.setPins(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);
  if (!SD_MMC.begin("/sdcard", true, true, SDMMC_FREQ_DEFAULT, 5)) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD_MMC card attached");
    return;
  }

  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);

  tft.pushImage(30, 60, logoWidth, logoHeight, logo_momysnow);

  const char* ssid = "YourSSID";  // Replace with your WiFi SSID
  const char* password = "YourPassword";  // Replace with your WiFi password

  if (!connect_wifi(ssid, password)) {
    Serial.println("Failed to connect to Wi-Fi, starting access point");
    start_ap();
    create_web_server();
    //tft.drawString("Cubot", 40, 40, 5);
    //tft.drawString("Psw: Cubot1234", 30, 60, 10);
  } else {
    Serial.println("Connected to Wi-Fi");
    create_web_server();
    //tft.drawString("Connected to Wi-Fi", 40, 40, 4);
  }

  tft.fillScreen(TFT_BLACK);
  delay(500);
  eyes.createSprite(240, 240);
}

void handleClientTask(void *param) {
  while (true) {
    // Handle client requests here
    server.handleClient();
    vTaskDelay(10); // Delay to yield to other tasks
  }
}

void loop() {
  eyes.fillEllipse(x_eyeL, y_eyeL, w_eyes, h_eyes, TFT_WHITE);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes, TFT_WHITE);
  eyes.pushSprite(0, 0);
  eyes.fillEllipse(x_eyeL, y_eyeL, w_eyes, h_eyes, TFT_BLACK);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes, TFT_BLACK);
  delay(1000);
  wink_eyes();

  dht.readTemperature();
  dht.readHumidity();
  delay(50);
}

bool connect_wifi(const char* ssid, const char* password) {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 15) {
    delay(500);
    Serial.print(".");
    retries++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    return true;
  } else {
    Serial.println("");
    Serial.println("WiFi connection failed");
    return false;
  }
}

void scan_wifi() {
  int n = WiFi.scanNetworks();
  Serial.println("Scan done");
  if (n == 0) {
    Serial.println("No networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println("");
      delay(10);
    }
  }
}

void start_ap() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP("Cubot", "Cubot1234");
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
}

void create_web_server() {
  server.on("/", HTTP_GET, handle_root);
  // Define other routes here

  server.begin();
  Serial.println("Web server started");
}



void onRoot(void (*handler)()) {
  server.on("/", handler);
}

void onNotFound(void (*handler)()) {
  server.onNotFound(handler);
}

void handle_root() {
  String html = "<html><body><h1>ESP32 Web Server</h1>";
  server.send(200, "text/html", html);
}