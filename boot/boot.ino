#include "setup.h"

#include <DHT.h>
#include "sd_read_write.h"
#include "SD_MMC.h"
#include <ESP32Servo.h>

#include "animation.h"

#include "microphone.h"

#include <Arduino.h>
#include <EEPROM.h>

// Indice nella EEPROM dove il valore sarà memorizzato
int address = 0;
// Leggi il valore dalla EEPROM
int touchPadThreshold = EEPROM.read(address);

#include <WiFiManager.h>
#include <ArduinoOTA.h>

//ADDED*******************************************
#include <PubSubClient.h>
#include <SimpleTimer.h>
//************************************************

// task
TaskHandle_t Animation;
TaskHandle_t DHT11sensor;
TaskHandle_t TouchPad;

// wifi
const char* ota_host_name = "Momy-";
uint32_t chipId = 0;

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

//Added**********************************************************
//mqtt
// Add your MQTT Broker IP address, example:
const char* mqtt_server = "192.168.1.121";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

// Create a mqtt checkin simple timer
SimpleTimer mqttCheckinTimer;
//****************************************************************

int pos = 0;
int lastPosition = -1;  // Initialized to an impossible value

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

  // wifi connection
  WiFi.mode(WIFI_STA);  // explicitly set mode, esp defaults to STA+AP
  //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;                        // Crea un oggetto WiFiManager
  wm.setConnectTimeout(90);              // Imposta il timeout di connessione a 90 secondi
  wm.setAPCallback(configModeCallback);  // Imposta la callback da chiamare quando il dispositivo è in modalità configurazione AP

  bool res;
  res = wm.autoConnect();  // anonymous ap

  // Provare a connettersi a tutte le reti WiFi memorizzate
  if (res) {
    // Se la connessione WiFi ha avuto successo
    Serial.println("");
    Serial.println("WiFi connected");  // Stampa un messaggio indicando la connessione WiFi avvenuta con successo
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());  // Stampa l'indirizzo IP assegnato al dispositivo

    // Ottieni l'ID del chip univoco per utilizzarlo come nome host, "Momy-" + chipId
    // Ottieni l'ID del chip
    for (int i = 0; i < 17; i = i + 8) {
      chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
    }

    String ota_host_name = "Momy-" + String(chipId);  // Crea il nome host per l'OTA (Over-The-Air) update
    ArduinoOTA.setHostname(ota_host_name.c_str());    // Imposta il nome host per l'OTA
    ArduinoOTA.begin();                               // Inizia il servizio OTA

    //TEXT
    String A = "Connected";
    String B = "to";
    String C = "Wi-Fi";
    int ATextWidth = tft.textWidth(A);  // Get the width of the large text
    int BTextWidth = tft.textWidth(B);  // Get the width of the large text
    int CTextWidth = tft.textWidth(C);  // Get the width of the large text

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
    tft.println(A);
    tft.setCursor(centerX1, centerY1);
    tft.println(B);
    tft.setCursor(centerX2, centerY2);
    tft.println(C);

    //TEXT
    A = "Name for";
    B = "update";
    C = ota_host_name;
    ATextWidth = tft.textWidth(A);  // Get the width of the large text
    BTextWidth = tft.textWidth(B);  // Get the width of the large text
    CTextWidth = tft.textWidth(C);  // Get the width of the large text

    largeTextHeight = tft.fontHeight();  // Get the height of the large text

    // Calculate the coordinates to center the text
    centerX = (tft.width() - ATextWidth) / 2;
    centerX1 = (tft.width() - BTextWidth) / 2;
    centerX2 = (tft.width() - CTextWidth) / 2;
    centerY = (tft.height() - largeTextHeight * 3) / 2;
    centerY1 = ((tft.height() - largeTextHeight * 3) / 2) + largeTextHeight;
    centerY2 = ((tft.height() - largeTextHeight * 3) / 2) + largeTextHeight * 2;

    tft.fillScreen(TFT_BLACK);

    // Print the large text centered on the screen
    tft.setCursor(centerX, centerY);
    tft.println(A);
    tft.setCursor(centerX1, centerY1);
    tft.println(B);
    tft.setCursor(centerX2, centerY2);
    tft.println(C);

    //ADDED**************************************************************
    //mqtt
    client.setServer(mqtt_server, 1883);
    client.setCallback(mqttcallback);
    //*******************************************************************
    
    // Procedi con altre operazioni di setup/inizializzazione
  }


  // setup end
  tft.unloadFont();  // Remove the font to recover memory used

  tft.fillScreen(TFT_BLACK);
  delay(500);
  eyes.createSprite(240, 240);

  // create TASK
  // create a task Animation
  xTaskCreatePinnedToCore(AnimationTask, "Animation", 10000, NULL, 9, &Animation, 0);
  // create a task DHT11
  xTaskCreatePinnedToCore(DHT11Task, "DHT11", 10000, NULL, 1, &DHT11sensor, 0);
  // create a task TouchPad
  xTaskCreatePinnedToCore(TouchTask, "TouchPad", 500, NULL, 1, &TouchPad, 0);
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
    if (touchRead(touchPadHeadPin) > touchPadThreshold) {
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
      esp_sleep_enable_timer_wakeup(300000000);                         // Set the timer for 5 minutes
      esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);  // Reactivate RTC
      esp_light_sleep_start();
      tft.fillScreen(TFT_BLACK);
    }

    delay(300000);  // Wait 5 minutes before reading again
  }
}

void loop() {
  //ADDED*****************************************************************
  ArduinoOTA.handle(); // OTA programming handle
  if (!client.connected()) { //mqtt connection handling
    reconnect();
  } else if (client.connected() && mqttCheckinTimer.isReady()) {                  // Check is ready a timer, send checkin to MQTT server
    client.publish("momy-bot/checkIn", "OK");
    mqttCheckinTimer.reset();                        // Reset a second timer
  }
  client.loop();
  //************************************************************************
  delay(50);
}

void configModeCallback(WiFiManager* myWiFiManager) {

  Serial.println("Failed to connect");  // Stampa un messaggio in caso di fallimento della connessione WiFi

  // Ottieni il nome SSID dell'access point creato
  String apSSID = myWiFiManager->getConfigPortalSSID();

  //TEXT
  String A = "Failed";
  String B = "to";
  String C = "connect";
  int ATextWidth = tft.textWidth(A);  // Get the width of the large text
  int BTextWidth = tft.textWidth(B);  // Get the width of the large text
  int CTextWidth = tft.textWidth(C);  // Get the width of the large text

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
  tft.println(A);
  tft.setCursor(centerX1, centerY1);
  tft.println(B);
  tft.setCursor(centerX2, centerY2);
  tft.println(C);

  //TEXT
  A = "Connect";
  B = "to";
  C = apSSID;
  ATextWidth = tft.textWidth(A);  // Get the width of the large text
  BTextWidth = tft.textWidth(B);  // Get the width of the large text
  CTextWidth = tft.textWidth(C);  // Get the width of the large text

  largeTextHeight = tft.fontHeight();  // Get the height of the large text

  // Calculate the coordinates to center the text
  centerX = (tft.width() - ATextWidth) / 2;
  centerX1 = (tft.width() - BTextWidth) / 2;
  centerX2 = (tft.width() - CTextWidth) / 2;
  centerY = (tft.height() - largeTextHeight * 3) / 2;
  centerY1 = ((tft.height() - largeTextHeight * 3) / 2) + largeTextHeight;
  centerY2 = ((tft.height() - largeTextHeight * 3) / 2) + largeTextHeight * 2;

  tft.fillScreen(TFT_BLACK);

  // Print the large text centered on the screen
  tft.setCursor(centerX, centerY);
  tft.println(A);
  tft.setCursor(centerX1, centerY1);
  tft.println(B);
  tft.setCursor(centerX2, centerY2);
  tft.println(C);

  unsigned long startTime = millis();

  // Check if there are connected stations
  while (WiFi.softAPgetStationNum() == 0 && (millis() - startTime < 60000)) {
    // Wait for 100 milliseconds
    delay(100);
  }

  // If no client is connected after 60 seconds, restart the device
  if (WiFi.softAPgetStationNum() == 0) {
    Serial.println("Nessun client connesso entro 60 secondi. Riavvio del dispositivo...");
    delay(1000);  // Wait for a second before restarting
    ESP.restart();
  }

  tft.fillScreen(TFT_BLACK);
}
//ADDED*************************************************************************************
void reconnect() { //MQTT reconnect handle
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("momy-bot")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("momy-bot/eyes");
      Serial.println("MQTT Checkin Timer set for 5 minutes");
      // Set checkin timer
      mqttCheckinTimer.setInterval(300000); // 5 minutes
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void mqttcallback(char* topic, byte* message, unsigned int length) {  //MQTT handle topic digestions
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "momy-bot/eyes") {
    Serial.print("Changing output to ");
    if(messageTemp == "wink"){
      Serial.println("wink");
      wink_eyes();
      delay(200);
    }
    else if(messageTemp == "sleep"){
      Serial.println("sleep");
      sleep_eyes();
      delay(750);
    }
    else if(messageTemp == "close"){
      Serial.println("close");
      close_eyes();
      delay(500);
    }
    else {
      Serial.println(" -- UNKNOWN");
    }
  }
}
//*********************************************************************************************************************
