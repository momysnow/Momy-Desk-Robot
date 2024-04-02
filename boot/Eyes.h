#ifndef EYES_H
#define EYES_H

#include "TFT_eSPI.h"
#include "DisplayTools.h"

extern TFT_eSPI tft;

class Eyes : public DisplayTools{

  private:
    TFT_eSprite eyes;
    int w_eyes = 40;
    int h_eyes = 70;
    int h_eyes_close = 10;
    int eyes_distance = 30; 
    int x_eyeL, x_eyeR, y_eyes;

    bool pupil = false;
    int pupil_size = 8;

    void begin();
    void draw_eye(int x, int y, int h = 0);
    void draw_eyes(int h = 0);
    void draw_eyes_rect();
    void clear_sprite(int h = 0);

  public:
    Eyes(TFT_eSPI *tft, bool initialDarkMode);
    ~Eyes();
    void idle_eyes();
    void close_eyes();
    void wink_eyes();
    void sleep_eyes();

};

#endif