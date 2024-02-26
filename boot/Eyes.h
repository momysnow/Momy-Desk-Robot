#ifndef EYES_H
#define EYES_H

#include "DisplayTools.h"
extern TFT_eSPI tft;

class Eyes : public DisplayTools{
  private:
    TFT_eSprite eyes;
    int w_eyes = 40;
    int h_eyes = 70;
    int h_eyes_close = 10;
    int eyes_distance = 30; 
    int pupil_size = 8;
    int x_eyeL, x_eyeR, y_eyes;
    void draw_eye(int x, int y, int h = 0);
    void draw_eyes(int h = 0, bool pupil = true);
    void draw_pupil(uint8_t rgb[3], bool left_eye = true, bool right_eye = true);
    void clear_sprite(int h = 0);
    void eyes_rect();

  public:
    Eyes(TFT_eSPI *tft);
    void idle_eyes();
    void close_eyes();
    void wink_eyes();
    void sleep_eyes();

};

#endif