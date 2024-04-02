#include "setup.h"
// #include "driver/temp_sensor.h"
#include "sd_read_write.h"
#include <Wire.h>
#include "Servo.h"
#include "DisplayTools.h"
#include "Eyes.h"
#include "microphone.h"
#include <PubSubClient.h>
#include <SimpleTimer.h>
#include <Arduino.h>
#include <Preferences.h>
#include "WiFiManager.h"
#include <ArduinoOTA.h>
#include <EEPROM.h>
#include <WiFiClient.h>
#include "CameraUploader.h"

//init tft
TFT_eSPI tft = TFT_eSPI();

// camera server stream
const char* server_ip = "192.168.1.123";
const int server_port = 8000;
CameraUploader camera(server_ip, server_port);

// touch
Preferences preferences;

// Indice nella EEPROM dove il valore sarà memorizzato
int address = 0;
// Leggi il valore dalla EEPROM
int touchPadThreshold = EEPROM.read(address);

// task
SemaphoreHandle_t taskSyncPinSemaphore;

TaskHandle_t Animation;
TaskHandle_t temp_sensor_handle;
TaskHandle_t TouchPad;
TaskHandle_t StreamVideo;
TaskHandle_t StreamAudio;
TaskHandle_t Emotion;

// display
DisplayTools display(&tft);
//init eyes
Eyes *eyes;

// wifi
WiFiManager wifiManager(&display);

void setup() {
  Serial.begin(115200);

  // setup Screen
  Serial.println("setup Screen");
  display.begin();

  // display logo
  Serial.println("Display logo");
  display.showLogo();
  
  // set dark mode (defaut true)
  // display.setDarkMode(false); 
  eyes = new Eyes(&tft, display.getDarkMode());

  // TEXT
  Serial.println("setup text");
  display.setupLargeText();

  delay(1000);
  display.writeThreeLinesOfText("Setting", "Up", "Devices");

  // touch
  Serial.println("setup touch");
  preferences.begin("touch", false);  // Apre le preferenze con il nome "touch", false indica che non è solo di lettura


  // setup temp_sensor
  // temp_sensor_config_t temp_sensor = TSENS_CONFIG_DEFAULT();
  // temp_sensor.dac_offset = TSENS_DAC_L2;  // TSENS_DAC_L2 is default; L4(-40°C ~ 20°C), L2(-10°C ~ 80°C), L1(20°C ~ 100°C), L0(50°C ~ 125°C)
  // temp_sensor_set_config(temp_sensor);
  // temp_sensor_start();

  // setup SD
  SDManager::initSD();

  // setup servo
  Serial.println("setup servo");
  Servo& servo = Servo::getInstance();
  servo.pwm->begin();
  servo.pwm->setOscillatorFrequency(27000000);
  servo.pwm->setPWMFreq(SERVO_FREQ);

  delay(2000);

  // wifi connection
  wifiManager.begin("Momy-");

  // only set up the camera AFTER we are connected to wifi!
  // setup camera
  Serial.println("setup camera");
  camera.begin();

  display.tearDownText();

  delay(500);

  // Creare il semaforo
  taskSyncPinSemaphore = xSemaphoreCreateBinary();

  // Inizializzare il semaforo con un valore iniziale di "libero"
  xSemaphoreGive(taskSyncPinSemaphore);

  // create TASK
  // create a task Animation
  xTaskCreatePinnedToCore(AnimationTask, "Animation", 10000, NULL, 9, &Animation, 1);
  // create a task temp_sensor
  // xTaskCreatePinnedToCore(temp_sensorTask, "temp_sensor", 8000, NULL, 1, &temp_sensor_handle, 1);
  // create a task TouchPad
  xTaskCreatePinnedToCore(TouchTask, "TouchPad", 8000, NULL, 1, &TouchPad, 1);
  // create a task StreamVideo
  xTaskCreatePinnedToCore(StreamVideoTask, "StreamVideo", 10000, NULL, 8, &StreamVideo, 1);
  // create a task StreamAudio
  xTaskCreatePinnedToCore(StreamAudioTask, "StreamAudio", 10000, NULL, 8, &StreamAudio, 1);
  // create a task emotion
  xTaskCreatePinnedToCore(EmotionTask, "Emotion", 10000, NULL, 8, &Emotion, 1);
}

void StreamAudioTask(void* param) {
  while (1) {
    uploadFile();
  }
}

void StreamVideoTask(void* param) {
  while (1) {
    camera.uploadImage();
  }
}

void AnimationTask(void* param) {
  while (1) {
    eyes->idle_eyes();
    delay(1000);
    eyes->wink_eyes();

    delay(200);
  }
}

void TouchTask(void* param) {
  // Legge il valore salvato precedentemente nella chiave "NTV"
  int NTV = preferences.getInt("NTV", 0);

  // Legge il valore salvato precedentemente nella chiave "TV"
  int TV = preferences.getInt("TV", 0);

  int touchPadThreshold = NTV + ((TV - NTV) / 2);

  while (1) {
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

void temp_sensorTask(void* param) {
  const int temperaturaCritica = 30;  // critical threshold for temperature

  while (1) {
    if (xSemaphoreTake(taskSyncPinSemaphore, portMAX_DELAY)) {
      float temperatura = 0;
      // temp_sensor_read_celsius(&temperatura);

      // Check if the temperature or humidity exceeds critical thresholds
      if (temperatura > temperaturaCritica) {
        // print image high temperature
        display.clear();
        // tft.pushImage(30, 60, high_temperatureWidth, high_temperatureHeight, high_temperature);

        vTaskDelay(2000);

        // it goes into rest mode
        esp_sleep_enable_timer_wakeup(300000000);                         // Set the timer for 5 minutes
        esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);  // Reactivate RTC
        esp_light_sleep_start();
        display.clear();
      }

      // Rilascio il semaforo
      xSemaphoreGive(taskSyncPinSemaphore);
    }

    vTaskDelay(300000);  // Attendi 5 minuti prima di leggere nuovamente
  }
}

void EmotionTask(void* param) {
  while (1) {

    vTaskDelay(300000);  // Attendi 5 minuti prima di leggere nuovamente
  }
}

void loop() {
  wifiManager.handle();
  delay(50);
}

void configModeCallback(WiFiManager* myWiFiManager) {

  Serial.println("Failed to connect");  // Stampa un messaggio in caso di fallimento della connessione WiFi

  // Ottieni il nome SSID dell'access point creato
  String apSSID = myWiFiManager->getConfigPortalSSID();

  //TEXT
  display.writeThreeLinesOfText("Failed", "to", "Connect");

  //TEXT
  display.setupSmallText();
  display.writeThreeLinesOfText("Connect", "to", apSSID);

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

  display.clear();
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
      eyes->wink_eyes();
      delay(200);
    }
    else if(messageTemp == "sleep"){
      Serial.println("sleep");
      eyes->sleep_eyes();
      delay(750);
    }
    else if(messageTemp == "close"){
      Serial.println("close");
      eyes->close_eyes();
      delay(500);
    }
    else {
      Serial.println(" -- UNKNOWN");
    }
  }
}