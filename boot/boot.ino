#include <TFT_eSPI.h>  // Hardware-specific library
// #include "FS.h"
// #include <SPIFFS.h>  // Include the SPIFFS library
#include <DHT.h>  // Include the DHT library

DHT dht(11, DHT11);
float temp = 0;
float humidity = 0;

unsigned int frameRate = 100;                    // Imposta il frame rate desiderato (frame al secondo)
unsigned int frameDuration = 1000 / frameRate;  // Durata di un frame in millisecondi

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

/*
struct Animation {
  unsigned long startTime = 0;
  unsigned long duration = 0;
  int currentFrame = 0;
  bool completed = false;
  bool (*animationFunction)(int frame, unsigned long duration);
};

Animation animations[1];

bool close_eyes(int frame, unsigned long duration) {
  int timeCloseEyes = 200;
  int totFrameTransition = duration - (timeCloseEyes / frameDuration);  // la durata tot in frame meno i frame necessari per 200ms di chiusura occhi

  while (totFrameTransition % 2 == 1) {
    totFrameTransition--;
  }

  int h_eye_frame = ((h_eyes - close_h_eyes) / ((totFrameTransition / 2) + 1));  // differenza tra l'altezza dell'occhio aperto e chiuso, diviso la prima metà dei frame + il frame dell'occhio aperto

  if (totFrameTransition / 2 > frame) {
    // avvio animazione
    eyes.fillEllipse(x_eyeL, y_eyeL, w_eyes, h_eyes - (h_eye_frame * frame), TFT_WHITE);  //l'altezza dell'occhio aperto meno l'altezza di ogni frame * il frame a cui siamo
    eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - (h_eye_frame * frame), TFT_WHITE);
  } else if ((totFrameTransition / 2) + (timeCloseEyes / frameDuration) < frame) {
    // conclusione animazione   
    eyes.fillEllipse(x_eyeL, y_eyeL, w_eyes, close_h_eyes + (h_eye_frame * ((frame - (totFrameTransition / 2)) - (timeCloseEyes / frameDuration))), TFT_WHITE);  // per l'altezza dell'occhio è l'altezza dell'occhio chiuso + l'altezza di ogni frame * il frame a cui siamo meno tutti i frame precedenti
    eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, close_h_eyes + (h_eye_frame * ((frame - (totFrameTransition / 2)) - (timeCloseEyes / frameDuration))), TFT_WHITE);
  } else {
    // animazione centrale
    // 0.2 sec
    // rettangolo
    eyes.fillRect(x_eyeL, y_eyeL, w_eyes, close_h_eyes, TFT_WHITE);
    eyes.fillRect(x_eyeR, y_eyeR, w_eyes, close_h_eyes, TFT_WHITE);
  }

  Serial.print("frame: ");
  Serial.println(frame);
  return frame >= duration;
}
*/

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
  /*
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
  }

  animations[0].animationFunction = close_eyes;
  animations[0].duration = 500 / frameDuration;
  animations[0].currentFrame = 0;
  animations[0].completed = false;
*/
  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);

  tft.pushImage(30, 60, logoWidth, logoHeight, logo_momysnow);
  delay(2000);

  tft.fillScreen(TFT_BLACK);
  delay(500);

  eyes.createSprite(240, 240);
}

void loop() {
  unsigned long currentMillis = millis();

  eyes.fillEllipse(x_eyeL, y_eyeL, w_eyes, h_eyes, TFT_WHITE);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(20);
  eyes.fillEllipse(x_eyeL, y_eyeL, w_eyes, h_eyes, TFT_BLACK);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes, TFT_BLACK);

  delay(1000);
/*
  if (animations[0].completed) {
    animations[0].startTime = currentMillis;
    animations[0].currentFrame = 0;
    animations[0].duration = 500 / frameDuration;
    animations[0].completed = false;
  }
*/

/*
  animations[0].completed = animations[0].animationFunction(animations[0].currentFrame, animations[0].duration);
*/

  wink_eyes();

/*
  if (currentMillis - animations[0].startTime >= (animations[0].currentFrame + 1) * frameDuration) {
    animations[0].currentFrame++;
  }
*/

  dht.readTemperature();
  dht.readHumidity();
  delay(50);
}
