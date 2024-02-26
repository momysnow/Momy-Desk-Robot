#include "Eyes.h"

uint8_t black[3] = {0,0,0};
uint8_t white[3] = {255,255,255};

/**
 * @brief Constructor for the Eyes class.
 * 
 * This constructor initializes an Eyes object with the specified parameters.
 * It configures the position of the eyes based on the screen's width and height,
 * adjusting the eye distance and dark mode accordingly.
 * The eye coordinates and other parameters are calculated for a centered layout.
 */

Eyes::Eyes(TFT_eSPI *tft): DisplayTools(tft), eyes(tft){
  x_eyeL = (w_screen - eyes_distance * 2 - w_eyes) / 2;
  x_eyeR = w_screen - x_eyeL;

  y_eyes = 30 + (h_screen - h_eyes) / 2;
}

/**
 * @brief Draws the pupil for one or both eyes.
 * 
 * @param rgb An array representing the RGB color values for the pupil.
 * @param left_eye Indicates whether to draw the pupil for the left eye.
 * @param right_eye Indicates whether to draw the pupil for the right eye.
 * 
 * This function draws the pupil for the specified eye(s) using the given RGB color values.
 * If 'left_eye' is true, it draws the pupil for the left eye at its calculated position.
 * If 'right_eye' is true, it draws the pupil for the right eye at its calculated position.
 * The pupil size is determined by the 'pupil_size' property of the Eyes object.
 */

void Eyes::draw_pupil(uint8_t rgb[3], bool left_eye, bool right_eye){
  if(left_eye){
    eyes.fillEllipse(x_eyeL, y_eyes, pupil_size, pupil_size, tft.color565(rgb[0], rgb[1], rgb[2]));
  }
  if(right_eye){
    eyes.fillEllipse(x_eyeR, y_eyes, pupil_size, pupil_size, tft.color565(rgb[0], rgb[1], rgb[2]));
  }
}

/**
 * @brief Draws an eye at the specified position with a modified height.
 * 
 * @param x X-coordinate of the eye.
 * @param y Y-coordinate of the eye.
 * @param h Additional height modification for the eye.
 * 
 * This function draws an eye at the given coordinates with a modified height,
 * allowing for customization of the eye appearance.
 */

void Eyes::draw_eye(int x, int y, int h){
  if (h < -h_eyes) {
    eyes_rect();
    draw_pupil(white, false, false);
  }else{
    eyes.fillEllipse(x, y, w_eyes, h_eyes + h, (dark_mode ? TFT_WHITE : TFT_BLACK));
  }
}

/**
 * @brief Draws eyes on the screen with optional pupils and customized height.
 * 
 * @param h Additional height modification for the eyes.
 * @param pupil Indicates whether to draw pupils.
 * 
 * This function draws eyes on the screen with an optional height modification,
 * and the option to include pupils. The appearance is adjusted based on the dark mode setting.
 */

void Eyes::draw_eyes(int h, bool pupil){
  if(dark_mode){
    eyes.fillScreen(TFT_BLACK);
    draw_eye(x_eyeR, y_eyes, h);
    draw_eye(x_eyeL, y_eyes, h);
    if(pupil && h > -h_eyes) draw_pupil(black);
  }else{
    eyes.fillScreen(TFT_WHITE);
    draw_eye(x_eyeR, y_eyes, h);
    draw_eye(x_eyeL, y_eyes, h);
    if(pupil && h > -h_eyes) draw_pupil(white);
  }
}

/**
 * @brief Clears the eye area by filling it with the background color.
 * 
 * @param h Additional height modification for clearing the eye area.
 * 
 * This function clears the eye area by filling it with the background color,
 * adjusting the appearance based on the dark mode setting.
 */

void Eyes::clear_sprite(int h){
  if(dark_mode){
    eyes.fillEllipse(x_eyeL, y_eyes, w_eyes, h_eyes + h, TFT_BLACK);
    eyes.fillEllipse(x_eyeR, y_eyes, w_eyes, h_eyes + h, TFT_BLACK);
  }else{
    eyes.fillEllipse(x_eyeL, y_eyes, w_eyes, h_eyes + h, TFT_WHITE);
    eyes.fillEllipse(x_eyeR, y_eyes, w_eyes, h_eyes + h, TFT_WHITE);
  }
}


/**
 * @brief Animates a rectangle representing closed eyes on the screen.
 * 
 * This function animates a rectangle representing closed eyes on the screen,
 * providing a blinking effect. The appearance is adjusted based on the dark mode setting.
 */

void Eyes::eyes_rect(){
  if(dark_mode){
    eyes.fillRoundRect(x_eyeL - 40, y_eyes - 5, w_eyes + 40, h_eyes_close, 4, TFT_WHITE);
    eyes.fillRoundRect(x_eyeR - 40, y_eyes - 5, w_eyes + 40, h_eyes_close, 4, TFT_WHITE);
  }else{
    eyes.fillRoundRect(x_eyeL - 40, y_eyes - 5, w_eyes + 40, h_eyes_close, 4, TFT_BLACK);
    eyes.fillRoundRect(x_eyeR - 40, y_eyes - 5, w_eyes + 40, h_eyes_close, 4, TFT_BLACK);
  }
}

/**
 * @brief Creates an idle state for the eyes on the screen.
 * 
 * This function creates an idle state for the eyes on the screen,
 * displaying them without any specific animation.
 */

void Eyes::idle_eyes(){
  eyes.createSprite(240, 240);
  draw_eyes();
  eyes.pushSprite(0, 0);
  clear_sprite();
}

/**
 * @brief Animates the closing and opening of the eyes.
 * 
 * This function animates the closing and opening of the eyes on the screen,
 * creating a blinking effect with a customizable speed.
 */

void Eyes::close_eyes() {
  // close
  for (int i = 0; i < 20; i++) {
    draw_eyes(i * -4);
    eyes.pushSprite(0, 0);
    delay(1);
    clear_sprite(i * -4);
  }
  // open
  for (int i = 20; i >= 0; i--) {
    draw_eyes(i * -4);
    eyes.pushSprite(0, 0);
    delay(1); 
    clear_sprite(i * -4);
  }
}

/**
 * @brief Wink animation
 * 
 * This function allows you to perform a wink animation.
 */

void Eyes::wink_eyes() {
  //close
  for (int i = 0; i < 20; i++) {
    draw_eye(x_eyeL, y_eyes, i * -4);
    draw_eye(x_eyeR, y_eyes);
    draw_pupil(black, true, true);
    eyes.pushSprite(0, 0);
    delay(1);
    clear_sprite(i * -4);
  }
  //open
  for (int i = 20; i >= 0; i--) {
    draw_eye(x_eyeL, y_eyes, i * -4);
    draw_eye(x_eyeR, y_eyes);
    draw_pupil(black, true, true);
    eyes.pushSprite(0, 0);
    delay(1); 
    clear_sprite(i * -4);
  }
  
}

/**
 * @brief Animates the eyes gradually closing and opening to simulate falling asleep.
 * 
 * This function animates the eyes gradually closing and opening to simulate falling asleep,
 * creating a drowsy effect with a customizable speed.
 */
void Eyes::sleep_eyes(){
  for (int i = 0; i < 20; i++) {
    draw_eyes(i * -4);
    eyes.pushSprite(0, 0);
    delay(1);
  }
  eyes_rect();
}