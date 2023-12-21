#ifndef ANIMATION_H
#define ANIMATION_H

#include <TFT_eSPI.h>
#include "Logo.h"
#include "high_temperature.h"
#include "NotoSansBold15.h"
#include "NotoSansBold36.h"
// The font names are arrays references, thus must NOT be in quotes ""
#define AA_FONT_SMALL NotoSansBold15
#define AA_FONT_LARGE NotoSansBold36

extern TFT_eSPI tft;
extern TFT_eSprite eyes;

extern const unsigned int sreenW;
extern const unsigned int sreenH;

extern int w_eyes;
extern int h_eyes;
extern int close_h_eyes;
extern int eyes_distance;
extern int x_eyeL;
extern int y_eyeL;
extern int x_eyeR;
extern int y_eyeR;

extern void idle_eyes();
extern void close_eyes();
extern void wink_eyes();
extern void sleep_eyes();

#endif
