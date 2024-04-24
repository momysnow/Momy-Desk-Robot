#include "Eyes.h"

// Constructor ✅
Eyes::Eyes(TFT_eSPI *tft, bool initialDarkMode): DisplayTools(tft), eyes(tft){
  this->dark_mode = initialDarkMode;
  x_eyeL = (w_screen - eyes_distance * 2 - w_eyes) / 2;
  x_eyeR = w_screen - x_eyeL;

  y_eyes = 30 + (h_screen - h_eyes) / 2;
}


// ✅
void Eyes::begin(){
  clear_sprite();
  eyes.createSprite(240, 240);
  dark_mode ? eyes.fillScreen(TFT_BLACK) : eyes.fillScreen(TFT_WHITE);
}

// Draw eye ✅
void Eyes::draw_eye(int x, int y, int h){
  bool pupilState;
  if(h < -h_eyes){
    pupilState = false;
    draw_eyes_rect();
  }else{
    pupilState = pupil;
    eyes.fillEllipse(x, y, w_eyes, h_eyes + h, (dark_mode ? TFT_WHITE : TFT_BLACK));
    if(pupilState) eyes.fillEllipse(x, y, pupil_size, pupil_size, (dark_mode ? TFT_WHITE : TFT_BLACK));
  }
}

// Draw 2 eyes ✅
void Eyes::draw_eyes(int h){
  draw_eye(x_eyeR, y_eyes, h);
  draw_eye(x_eyeL, y_eyes, h);
}

// clear ✅
void Eyes::clear_sprite(int h){
  if(dark_mode){
    eyes.fillEllipse(x_eyeL, y_eyes, w_eyes, h_eyes + h, TFT_BLACK);
    eyes.fillEllipse(x_eyeR, y_eyes, w_eyes, h_eyes + h, TFT_BLACK);
  }else{
    eyes.fillEllipse(x_eyeL, y_eyes, w_eyes, h_eyes + h, TFT_WHITE);
    eyes.fillEllipse(x_eyeR, y_eyes, w_eyes, h_eyes + h, TFT_WHITE);
  }
}


// Rect (closing eye) ✅
void Eyes::draw_eyes_rect(){
  if(dark_mode){
    eyes.fillRoundRect(x_eyeL - 40, y_eyes - 5, w_eyes + 40, h_eyes_close, 4, TFT_WHITE);
    eyes.fillRoundRect(x_eyeR - 40, y_eyes - 5, w_eyes + 40, h_eyes_close, 4, TFT_WHITE);
  }else{
    eyes.fillRoundRect(x_eyeL - 40, y_eyes - 5, w_eyes + 40, h_eyes_close, 4, TFT_BLACK);
    eyes.fillRoundRect(x_eyeR - 40, y_eyes - 5, w_eyes + 40, h_eyes_close, 4, TFT_BLACK);
  }
}

// Idle eyes ✅
void Eyes::idle_eyes(){
  begin();
  draw_eye(x_eyeL, y_eyes);
  draw_eye(x_eyeR, y_eyes);
  // if(pupil) draw_pupil(true, true);
  eyes.pushSprite(0, 0);
  clear_sprite();
}

// Closing/Opening animation ✅
void Eyes::close_eyes() {
  begin();
  // close
  for (int i = 0; i < 20; i++) {
    draw_eyes(i * -4);
    eyes.pushSprite(0, 0);
    delay(1);
    clear_sprite();
  }
  // open
  for (int i = 20; i >= 0; i--) {
    draw_eyes(i * -4);
    eyes.pushSprite(0, 0);
    delay(1); 
    clear_sprite();
  }
}

// Wink animation ✅

void Eyes::wink_eyes() {
  begin();
    //close
  for (int i = 0; i < 20; i++) {
    draw_eye(x_eyeL, y_eyes, i * -4);
    draw_eye(x_eyeR, y_eyes);
    eyes.pushSprite(0, 0);
    delay(1);
    clear_sprite();
  }
  //open
  for (int i = 20; i >= 0; i--) {
    draw_eye(x_eyeL, y_eyes, i * -4);
    draw_eye(x_eyeR, y_eyes);
    eyes.pushSprite(0, 0);
    delay(1); 
    clear_sprite();
  }
}

// Sleep eyes ✅
void Eyes::sleep_eyes(){
  begin();
  for (int i = 0; i < 20; i++) {
    draw_eyes(i * -4);
    eyes.pushSprite(0, 0);
    delay(1);
    clear_sprite();
  }
}

// Deconstructor ✅
Eyes::~Eyes(){
  clear_sprite();
  eyes.deleteSprite();
}