#include <TFT_eSPI.h>  // Hardware-specific library
#include <DHT.h>  // Include the DHT library
#include "sd_read_write.h"
#include "SD_MMC.h"
#include <WiFi.h>
#include "WifiManager.h"

WifiManager wifiManager;

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

  SD_MMC.setPins(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);
  if (!SD_MMC.begin("/sdcard", true, true, SDMMC_FREQ_DEFAULT, 5)) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE){
    Serial.println("No SD_MMC card attached");
    return;
  }
  
  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);

  tft.pushImage(30, 60, logoWidth, logoHeight, logo_momysnow);
  delay(2000);

  tft.fillScreen(TFT_BLACK);
  delay(500);

  if (!wifiManager.connect_wifi("ssid", "password")) {
    Serial.println("Failed to connect to Wi-Fi, starting access point");
    wifiManager.start_ap();
    wifiManager.create_web_server();
    tft.drawString("Cubot",40,40,5);
    tft.drawString("Psw: Cubot1234",30,60,10);
  } else {
    Serial.println("Connected to Wi-Fi");
    wifiManager.create_web_server();
    tft.drawString("GAME OVER",40,40,4);
  }

  eyes.createSprite(240, 240);
}

void loop() {
  wifiManager.handleClient();

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