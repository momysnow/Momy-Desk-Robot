#include "Animation.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite eyes = TFT_eSprite(&tft);

const unsigned int sreenW = 240;
const unsigned int sreenH = 240;

int w_eyes = 40;
int h_eyes = 70;
int close_h_eyes = 10;
int eyes_distance = 30;
int x_eyeL = (sreenW - eyes_distance * 2 - w_eyes) / 2;
int y_eyeL = 30 + (sreenH - h_eyes) / 2;
int x_eyeR = sreenW - ((sreenW - eyes_distance * 2 - w_eyes) / 2);
int y_eyeR = 30 + (sreenH - h_eyes) / 2;

void idle_eyes() {

  // start animation
  eyes.fillEllipse(x_eyeL, y_eyeL, w_eyes, h_eyes, TFT_WHITE);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes, TFT_WHITE);
  eyes.pushSprite(0, 0);
  eyes.fillEllipse(x_eyeL, y_eyeL, w_eyes, h_eyes, TFT_BLACK);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes, TFT_BLACK);
}

void close_eyes() {

  // start animation
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
  eyes.fillRect(x_eyeL - 30, y_eyeL, w_eyes + 30, close_h_eyes, TFT_WHITE);
  eyes.fillRect(x_eyeR - 30, y_eyeR, w_eyes + 30, close_h_eyes, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(350);
  eyes.fillRect(x_eyeL - 30, y_eyeL, w_eyes + 30, close_h_eyes, TFT_BLACK);
  eyes.fillRect(x_eyeR - 30, y_eyeR, w_eyes + 30, close_h_eyes, TFT_BLACK);

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

  // start animation
  eyes.fillEllipse(x_eyeL, y_eyeL, w_eyes, h_eyes, TFT_WHITE);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(10);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes, TFT_BLACK);

  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 5, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(10);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 5, TFT_BLACK);

  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 15, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(10);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 15, TFT_BLACK);

  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 40, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(10);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 40, TFT_BLACK);

  // animazione centrale
  // 0.2 sec
  // rettangolo
  eyes.fillRect(x_eyeR - 30, y_eyeR, w_eyes + 30, close_h_eyes, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(350);
  eyes.fillRect(x_eyeR - 30, y_eyeR, w_eyes + 30, close_h_eyes, TFT_BLACK);

  // conclusione animazione
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 40, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(10);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 40, TFT_BLACK);

  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 15, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(10);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 15, TFT_BLACK);

  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 5, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(10);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes - 5, TFT_BLACK);

  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(10);
  eyes.fillEllipse(x_eyeL, y_eyeL, w_eyes, h_eyes, TFT_BLACK);
  eyes.fillEllipse(x_eyeR, y_eyeR, w_eyes, h_eyes, TFT_BLACK);
}

void sleep_eyes() {
  // animazione centrale
  // 0.2 sec
  // rettangolo
  eyes.fillRect(x_eyeR - 30, y_eyeR, w_eyes + 30, close_h_eyes, TFT_WHITE);
  eyes.pushSprite(0, 0);
  delay(350);
  eyes.fillRect(x_eyeR - 30, y_eyeR, w_eyes + 30, close_h_eyes, TFT_BLACK);
}