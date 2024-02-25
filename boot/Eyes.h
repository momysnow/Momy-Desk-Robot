#ifndef EYES_H
#define EYES_H

#include <TFT_eSPI.h>
#include "NotoSansBold15.h"
#include "NotoSansBold36.h"
// The font names are arrays references, thus must NOT be in quotes ""
#define AA_FONT_SMALL NotoSansBold15
#define AA_FONT_LARGE NotoSansBold36

class Eyes{
  private:
    TFT_eSprite eyes;
    int w_eyes = 40;
    int h_eyes = 70;
    int h_eyes_close = 10;
    int eyes_distance = 30; 
    int pupil_size = 8;
    int x_eyeL, x_eyeR, y_eyes, w_screen, h_screen;
    bool dark_mode;
    void draw_eye(int x, int y, int h = 0);
    void draw_eyes(int h = 0, bool circle = true);
    void draw_pupil(uint8_t rgb[3], bool left_eye = true, bool right_eye = true);
    void clear_sprite(int h = 0);
    void eyes_rect();

  public:
    TFT_eSPI tft;
    Eyes(TFT_eSPI *tft, bool dark_mode = true, unsigned int w_screen = 240, unsigned int h_screen = 240);
    void idle_eyes();
    void close_eyes();
    void wink_eyes();
    void sleep_eyes();

};

#endif