#include "setup.h"

#include <DHT.h>
#include "sd_read_write.h"
#include "SD_MMC.h"
#include <ESP32Servo.h>

#include "esp_camera.h"
const char *server_ip = "192.168.1.123";  // Sostituisci con l'indirizzo IP del tuo server Python
const int server_port = 8000;             // Sostituisci con la porta del tuo server Python

int frameRate = 30;


#include "animation.h"

#include "microphone.h"

#include <Arduino.h>

#include <Preferences.h>
Preferences preferences;

#include <WiFiManager.h>
#include <ArduinoOTA.h>

// task
SemaphoreHandle_t taskSyncPinSemaphore;

TaskHandle_t Animation;
TaskHandle_t DHT11sensor;
TaskHandle_t TouchPad;
TaskHandle_t Streaming;

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

  //camera
  cameraSetup();
  //touch
  preferences.begin("touch", false);  // Apre le preferenze con il nome "touch", false indica che non è solo di lettura
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
  ESP32PWM::allocateTimer(4);
  ESP32PWM::allocateTimer(5);
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

  // Ottieni l'ID del chip univoco per utilizzarlo come nome host, "Momy-" + chipId
  // Ottieni l'ID del chip
  for (int i = 0; i < 17; i = i + 8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  // HOST
  String host_name = "Momy-" + String(chipId);  // Crea il nome host

  bool res;
  res = wm.autoConnect(host_name.c_str());  // ap

  // Provare a connettersi a tutte le reti WiFi memorizzate
  if (res) {
    // Se la connessione WiFi ha avuto successo
    Serial.println("");
    Serial.println("WiFi connected");  // Stampa un messaggio indicando la connessione WiFi avvenuta con successo
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());  // Stampa l'indirizzo IP assegnato al dispositivo

    ArduinoOTA.setHostname(host_name.c_str());  // Imposta il nome host per l'OTA
    ArduinoOTA.begin();                         // Inizia il servizio OTA

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

    // Procedi con altre operazioni di setup/inizializzazione
  }


  // setup end
  tft.unloadFont();  // Remove the font to recover memory used

  tft.fillScreen(TFT_BLACK);
  delay(500);
  eyes.createSprite(240, 240);


  // Creare il semaforo
  taskSyncPinSemaphore = xSemaphoreCreateBinary();

  // Inizializzare il semaforo con un valore iniziale di "libero"
  xSemaphoreGive(taskSyncPinSemaphore);

  // create TASK
  // create a task Animation
  xTaskCreatePinnedToCore(AnimationTask, "Animation", 15000, NULL, 9, &Animation, 1);
  // create a task DHT11
  xTaskCreatePinnedToCore(DHT11Task, "DHT11", 10000, NULL, 1, &DHT11sensor, 0);
  // create a task TouchPad
  xTaskCreatePinnedToCore(TouchTask, "TouchPad", 10000, NULL, 1, &TouchPad, 0);
  // create a task Streaming
  xTaskCreatePinnedToCore(StreamingTask, "Streaming", 30000, NULL, 8, &Streaming, 1);
}

void AnimationTask(void* param) {
  while (true) {
    idle_eyes();
    vTaskDelay(1000);
    wink_eyes();

    vTaskDelay(200);
  }
}

void TouchTask(void* param) {
  // Legge il valore salvato precedentemente nella chiave "NTV"
  int NTV = preferences.getInt("NTV", 0);

  // Legge il valore salvato precedentemente nella chiave "TV"
  int TV = preferences.getInt("TV", 0);

  int touchPadThreshold = NTV + ((TV - NTV) /2);

  while (true) {
    if (xSemaphoreTake(taskSyncPinSemaphore, portMAX_DELAY)) {
      // Controllo touch
      if (touchRead(touchPadHeadPin) > touchPadThreshold) {
        // Toccato
        // Animazione touch
        // Incremento di "happiness"
      }

      // Rilascio il semaforo
      xSemaphoreGive(taskSyncPinSemaphore);
    }

    vTaskDelay(100);
  }
}

void DHT11Task(void* param) {
  const int temperaturaCritica = 30;  // critical threshold for temperature
  const int umiditaCritica = 80;      // critical threshold for humidity

  while (true) {
    if (xSemaphoreTake(taskSyncPinSemaphore, portMAX_DELAY)) {
      float temperatura = dht.readTemperature();
      float umidita = dht.readHumidity();

      // Check if the temperature or humidity exceeds critical thresholds
      if (temperatura > temperaturaCritica || umidita > umiditaCritica) {
        // print image high temperature
        tft.fillScreen(TFT_BLACK);
        tft.pushImage(30, 60, high_temperatureWidth, high_temperatureHeight, high_temperature);

        vTaskDelay(2000);

        // it goes into rest mode
        esp_sleep_enable_timer_wakeup(300000000);                         // Set the timer for 5 minutes
        esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);  // Reactivate RTC
        esp_light_sleep_start();
        tft.fillScreen(TFT_BLACK);
      }

      // Rilascio il semaforo
      xSemaphoreGive(taskSyncPinSemaphore);
    }

    vTaskDelay(300000);  // Attendi 5 minuti prima di leggere nuovamente
  }
}

void StreamingTask(void* param) {
  //uxTaskGetStackHighWaterMark(NULL); // check how much of that space is/was used
  WiFiManagerParameter serverParam("server", "Server IP", server_ip, 15);
  WiFiManagerParameter portParam("port", "Server Port", String(server_port).c_str(), 6);

  // Add parameters to WiFiManager
  wm.addParameter(&serverParam);
  wm.addParameter(&portParam);

  // Try to connect to WiFi or start configuration portal
  if (wm.autoConnect()) {
    Serial.println("Connected to WiFi");

    // Retrieve the configured server IP and port
    strcpy(server_ip, serverParam.getValue());
    server_port = atoi(portParam.getValue());

    // Rest of your streaming task code...
    if (client.connect(server_ip, server_port)) {
      Serial.println("Connected to server");

      camera_fb_t *fb = esp_camera_fb_get();
      if (fb) {
        // Rest of your streaming task code...
      } else {
        Serial.println("Error capturing image from camera");
      }

      client.stop();
    } else {
      Serial.println("Unable to connect to server");
    }
  } else {
    Serial.println("Failed to connect to WiFi");
  }

  vTaskDelay(1000 / frameRate);
}

void loop() {
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

void cameraSetup() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_HVGA;
  config.pixel_format = PIXFORMAT_JPEG; // for streaming
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 10;
  config.fb_count = 1;
  
  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  // for larger pre-allocated frame buffer.
  if(psramFound()){
    config.jpeg_quality = 10;
    config.fb_count = 2;
    config.grab_mode = CAMERA_GRAB_LATEST;
  } else {
    // Limit the frame size when PSRAM is not available
    config.frame_size = FRAMESIZE_CIF;
    config.fb_location = CAMERA_FB_IN_DRAM;
  }

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  s->set_brightness(s, 0);     // -2 to 2
  s->set_contrast(s, 0);       // -2 to 2
  s->set_saturation(s, 0);     // -2 to 2
  s->set_special_effect(s, 0); // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
  s->set_whitebal(s, 1);       // 0 = disable , 1 = enable
  s->set_awb_gain(s, 1);       // 0 = disable , 1 = enable
  s->set_wb_mode(s, 0);        // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
  s->set_exposure_ctrl(s, 1);  // 0 = disable , 1 = enable
  s->set_aec2(s, 0);           // 0 = disable , 1 = enable
  s->set_ae_level(s, 0);       // -2 to 2
  s->set_aec_value(s, 300);    // 0 to 1200
  s->set_gain_ctrl(s, 1);      // 0 = disable , 1 = enable
  s->set_agc_gain(s, 0);       // 0 to 30
  s->set_gainceiling(s, (gainceiling_t)0);  // 0 to 6
  s->set_bpc(s, 0);            // 0 = disable , 1 = enable
  s->set_wpc(s, 1);            // 0 = disable , 1 = enable
  s->set_raw_gma(s, 1);        // 0 = disable , 1 = enable
  s->set_lenc(s, 1);           // 0 = disable , 1 = enable
  s->set_hmirror(s, 0);        // 0 = disable , 1 = enable
  s->set_vflip(s, 0);          // 0 = disable , 1 = enable
  s->set_dcw(s, 1);            // 0 = disable , 1 = enable
  s->set_colorbar(s, 0);       // 0 = disable , 1 = enable

  Serial.println("Camera configuration complete!");
}