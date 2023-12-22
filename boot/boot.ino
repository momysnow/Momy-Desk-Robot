#include "setup.h"

#include <DHT.h>
#include "sd_read_write.h"
#include "SD_MMC.h"
#include <ESP32Servo.h>

#include "animation.h"

#include "microphone.h"

#include "touchpad.h"

#include "features.h"

#include <Arduino.h>
#include <WiFi.h>
#include <EEPROM.h>
#include <WebServer.h>

// task
TaskHandle_t Client;
TaskHandle_t Animation;
TaskHandle_t DHT11sensor;
TaskHandle_t TouchPad;

// webserver
WebServer server(80);

// SD
#define SD_MMC_CMD 38  //Please do not modify it.
#define SD_MMC_CLK 39  //Please do not modify it.
#define SD_MMC_D0 40   //Please do not modify it.

// dht
DHT dht(dhtPin, DHT11);
float temp = 0;
float humidity = 0;

// servo
Servo headServo;
Servo pushLServo;
Servo pushRServo;
Servo rotateLServo;
Servo rotateRServo;
Servo baseServo;

int pos = 0;
int lastPosition = -1;  // Initialized to an impossible value

touchPad touchPadHaed;

void setup() {
  Serial.begin(115200);
  
// setup Screen
  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);
  // Show Logo
  tft.pushImage(30, 60, logoWidth, logoHeight, logo_momysnow);
  //TEXT
  tft.setTextColor(TFT_WHITE, TFT_BLACK);  // Set the font color AND the background color
  tft.setTextWrap(true);                   // Wrap on width
  // Load the large font
  tft.loadFont(AA_FONT_LARGE);

// setup dht
  dht.begin();

// create TASK
  // create a task Client
  xTaskCreatePinnedToCore(handleClientTask, "Client", 10000, NULL, 10, &Client, 0);
  // create a task Animation
  xTaskCreatePinnedToCore(AnimationTask, "Animation", 10000, NULL, 9,&Animation, 0);
  // create a task DHT11
  xTaskCreatePinnedToCore(DHT11Task, "DHT11", 10000, NULL, 1, &DHT11sensor, 0);
  // create a task TouchPad
  xTaskCreatePinnedToCore(TouchTask, "TouchPad", 500, NULL, 1, &TouchPad, 0);

// setup SD
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
  // create file if doesnt exist
  if (!existFile(SD_MMC, "/knownNetworks.csv")) {
    // non-existent file
    // create file
    writeFile(SD_MMC, "/knownNetworks.csv", "");
  }

// setup servo
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  // HEAD
  headServo.setPeriodHertz(50);
  headServo.attach(headServoPin, 1000, 2000);
  headServo.write(90);
  // PUSH_L
  pushLServo.setPeriodHertz(50);
  pushLServo.attach(pushLServoPin, 1000, 2000);
  pushLServo.write(90);
  // PUSH_R
  pushRServo.setPeriodHertz(50);
  pushRServo.attach(pushRServoPin, 1000, 2000);
  pushRServo.write(90);
  // ROTATE_L
  rotateLServo.setPeriodHertz(50);
  rotateLServo.attach(rotateLServoPin, 1000, 2000);
  rotateLServo.write(90);
  // ROTATE_R
  rotateRServo.setPeriodHertz(50);
  rotateRServo.attach(rotateRServoPin, 1000, 2000);
  rotateRServo.write(90);
  // BASE
  baseServo.setPeriodHertz(50);
  baseServo.attach(baseServoPin, 1000, 2000);
  baseServo.write(90);

  delay(2000);

// Setup touchPad
  touchPadInit(&touchPadHaed, touchPadHeadPin);

// wifi connection
  if (!connect_wifi()) {
    Serial.println("Failed to connect to Wi-Fi, starting access point");
    start_ap();
    create_web_server();
    //TEXT
    int ATextWidth = tft.textWidth("Cubot");      // Get the width of the large text
    int BTextWidth = tft.textWidth("Psw:");       // Get the width of the large text
    int CTextWidth = tft.textWidth("Cubot1234");  // Get the width of the large text

    int largeTextHeight = tft.fontHeight();  // Get the height of the large text

    // Calculate the coordinates to center the text
    int centerX = (tft.width() - ATextWidth) / 2;
    int centerX1 = (tft.width() - BTextWidth) / 2;
    int centerX2 = (tft.width() - CTextWidth) / 2;
    int centerY = (tft.height() - largeTextHeight * 3) / 2;
    int centerY1 = ((tft.height() - largeTextHeight * 3) / 2) + largeTextHeight;
    int centerY2 = ((tft.height() - largeTextHeight * 3) / 2) + largeTextHeight * 2;

    tft.fillScreen(TFT_BLACK);

    // Print the large text centered on the screen
    tft.setCursor(centerX, centerY);
    tft.println("Cubot");
    tft.setCursor(centerX1, centerY1);
    tft.println("Psw:");
    tft.setCursor(centerX2, centerY2);
    tft.println("Cubot1234");

    delay(5000);
  } else {
    Serial.println("Connected to Wi-Fi");
    create_web_server();

    //TEXT
    int ATextWidth = tft.textWidth("Connected");  // Get the width of the large text
    int BTextWidth = tft.textWidth("to");         // Get the width of the large text
    int CTextWidth = tft.textWidth("Wi-Fi");      // Get the width of the large text

    int largeTextHeight = tft.fontHeight();  // Get the height of the large text

    // Calculate the coordinates to center the text
    int centerX = (tft.width() - ATextWidth) / 2;
    int centerX1 = (tft.width() - BTextWidth) / 2;
    int centerX2 = (tft.width() - CTextWidth) / 2;
    int centerY = (tft.height() - largeTextHeight * 3) / 2;
    int centerY1 = ((tft.height() - largeTextHeight * 3) / 2) + largeTextHeight;
    int centerY2 = ((tft.height() - largeTextHeight * 3) / 2) + largeTextHeight * 2;

    tft.fillScreen(TFT_BLACK);

    // Print the large text centered on the screen
    tft.setCursor(centerX, centerY);
    tft.println("Connected");
    tft.setCursor(centerX1, centerY1);
    tft.println("to");
    tft.setCursor(centerX2, centerY2);
    tft.println("Wi-Fi");

    delay(5000);
  }

// setup end
  tft.unloadFont();  // Remove the font to recover memory used

  tft.fillScreen(TFT_BLACK);
  delay(500);
  eyes.createSprite(240, 240);
}

void handleClientTask(void* param) {
  while (true) {
    // Handle client requests here
    server.handleClient();
    //vTaskDelay(10); // Delay to yield to other tasks
    delay(50);
  }
}

void AnimationTask(void* param) {
  while (true) {
    idle_eyes();
    delay(1000);
    wink_eyes();

    delay(200);
  }
}

void TouchTask(void* param) {
  while (true) {
    touchPadScan(&touchPadHaed);
    if (touchPad1.state == 1) {
      //touched
      //touch animation
      //happiness++
    }
    delay(100);
  }
}

void DHT11Task(void* param) {
  const int temperaturaCritica = 30;  // critical threshold for temperature
  const int umiditaCritica = 80;      // critical threshold for humidity

  while (true) {
    float temperatura = dht.readTemperature();
    float umidita = dht.readHumidity();

    // Check if the temperature or humidity exceeds critical thresholds
    if (temperatura > temperaturaCritica || umidita > umiditaCritica) {
      // print image high temperature
      tft.fillScreen(TFT_BLACK);
      tft.pushImage(30, 60, high_temperatureWidth, high_temperatureHeight, high_temperature);

      delay(2000);

      // it goes into rest mode
      esp_sleep_enable_timer_wakeup(300000000);  // Set the timer for 5 minutes
      esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);  // Reactivate RTC
      esp_light_sleep_start();
      tft.fillScreen(TFT_BLACK);
    }

    delay(300000);  // Wait 5 minutes before reading again
  }
}

void loop() {
  delay(50);
}

bool connect_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Scan surrounding WiFi networks
  int numNetworks = WiFi.scanNetworks();
  if (numNetworks == 0) {
    Serial.println("No WiFi networks detected. Connection failed.");
    return false;
  }

  // Load known networks from the file
  File file = SD_MMC.open("/knownNetworks.csv", FILE_READ);
  if (!file) {
    Serial.println("Error opening knownNetworks.csv");
    return false;
  }

  String strongestSSID;
  int strongestSignal = -100;  // Initialize with a very low value

  // Processes the networks detected during the scan
  for (int i = 0; i < numNetworks; i++) {
    String currentSSID = WiFi.SSID(i);

    // Check if the detected network is present in the known data
    if (findSSIDInFile(SD_MMC, "/knownNetworks.csv", currentSSID.c_str())) {
      // Try connecting to the current network
      WiFi.begin(currentSSID.c_str(), "");  // Password not required for scanning
      int retries = 0;
      while (WiFi.status() != WL_CONNECTED && retries < 15) {
        delay(500);
        Serial.print(".");
        retries++;
      }

      // If the connection is successful, check the signal
      if (WiFi.status() == WL_CONNECTED) {
        int currentSignal = WiFi.RSSI();
        if (currentSignal > strongestSignal) {
          strongestSignal = currentSignal;
          strongestSSID = currentSSID;
        }
        WiFi.disconnect();
      }
    }
  }
  file.close();

  // Check if at least one of the known networks has been found and try to connect
  if (!strongestSSID.isEmpty()) {
    Serial.println("");
    Serial.print("Connecting to the strongest network: ");
    Serial.println(strongestSSID);
    WiFi.begin(strongestSSID.c_str(), "");  // Password not required for connection

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
    }
  }

  Serial.println("");
  Serial.println("No known WiFi networks found. Connection failed.");
  return false;
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
  server.on("/wifi", HTTP_GET, handle_wifi);
  server.on("/save", HTTP_POST, handle_save);

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

void handle_wifi() {
  String html = "<html><body><h1>Configura la rete Wi-Fi</h1>";
  html += "<form method='post' action='/save'>";
  html += "SSID: <input type='text' name='ssid'><br>";
  html += "Password: <input type='password' name='password'><br>";
  html += "<input type='submit' value='Salva'></form>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handle_save() {
  if (server.hasArg("ssid") && server.hasArg("password")) {
    String ssid = server.arg("ssid");
    String password = server.arg("password");

    // Chiama la funzione findSSIDInFile per cercare l'SSID nel file
    if (!findSSIDInFile(SD_MMC, "/knownNetworks.csv", ssid.c_str())) {
      // Fai qualcosa se l'SSID non è stato trovato
      // Append the new network to the knownNetworks file
      String dataToAppend = ssid + "," + password + "\n";
      appendFile(SD_MMC, "/knownNetworks.csv", dataToAppend.c_str());
    }
  }

  String html = "<html><body><h1>Credenziali Wi-Fi salvate con successo</h1></body></html>";
  server.send(200, "text/html", html);
}