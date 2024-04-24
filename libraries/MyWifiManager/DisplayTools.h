// DisplayTools.h
#ifndef DISPLAY_TOOLS_H
#define DISPLAY_TOOLS_H

// #include "Eyes.h"
#include "Logo.h"
#include <TFT_eSPI.h>
#include "NotoSansBold15.h"
#include "NotoSansBold36.h"
// The font names are arrays references, thus must NOT be in quotes ""
#define AA_FONT_SMALL NotoSansBold15
#define AA_FONT_LARGE NotoSansBold36

extern TFT_eSPI tft;

class DisplayTools {
  protected:
    bool dark_mode = true;
    int w_screen, h_screen;

  public:
    DisplayTools(TFT_eSPI *tft, unsigned int w_screen = 240, unsigned int h_screen = 240);
    void begin();
    void clear();
    void stop();

    void showLogo();

    void setupLargeText();
    void setDarkMode(bool dark_mode);
    bool getDarkMode() { return dark_mode; }
    void setupSmallText();
    void writeThreeLinesOfText(String A, String B, String C);
    void tearDownText();
};

// TODO: use the State object oriented pattern to decide what should be displayed

#endif