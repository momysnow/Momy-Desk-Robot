// DisplayTools.cpp
#include "TFT_eSPI.h"
#include "DisplayTools.h"
#include <math.h>

DisplayTools::DisplayTools(TFT_eSPI *tft, unsigned int w_screen, unsigned int h_screen) {
  this->w_screen = w_screen;
  this->h_screen = h_screen;
}

void DisplayTools::begin() {
  tft.begin();
  tft.setRotation(0);
  dark_mode ? tft.fillScreen(TFT_BLACK) : tft.fillScreen(TFT_WHITE);
  tft.setSwapBytes(true);

  dark_mode ? tft.setTextColor(TFT_WHITE, TFT_BLACK) : tft.setTextColor(TFT_BLACK, TFT_WHITE);  // Set the font color AND the background color
  tft.setTextWrap(true);  // Wrap on width
}


void DisplayTools::setDarkMode(bool dark_mode){
  this->dark_mode = dark_mode;
}

void DisplayTools::setupLargeText(){
  // Load the large font
  tft.loadFont(AA_FONT_LARGE);
}

void DisplayTools::setupSmallText(){
  tft.loadFont(AA_FONT_SMALL);
}

void DisplayTools::tearDownText(){
   // Remove the font to recover memory used
  tft.unloadFont(); 
  dark_mode ? tft.fillScreen(TFT_BLACK) : tft.fillScreen(TFT_WHITE);
}

void DisplayTools::clear(){
  dark_mode ? tft.fillScreen(TFT_BLACK) : tft.fillScreen(TFT_WHITE);

}

void DisplayTools::stop(){

}

void DisplayTools::showLogo(){
  // Show Logo
  tft.pushImage(30, 60, logoWidth, logoHeight, logo_momysnow);
}

void DisplayTools::writeThreeLinesOfText(String A, String B, String C){
    int ATextWidth = tft.textWidth(A);  // Get the width of the large text
    int BTextWidth = tft.textWidth(B);  // Get the width of the large text
    int CTextWidth = tft.textWidth(C);  // Get the width of the large text

    int largeTextHeight = tft.fontHeight();  // Get the height of the large text

    // Calculate the coordinates to center the text
    int centerX = (tft.width() - ATextWidth) / 2;
    int centerX1 = (tft.width() - BTextWidth) / 2;
    int centerX2 = (tft.width() - CTextWidth) / 2;
    int centerY = (tft.height() - largeTextHeight * 3) / 2;
    int centerY1 = ((tft.height() - largeTextHeight * 3) / 2) + largeTextHeight;
    int centerY2 = ((tft.height() - largeTextHeight * 3) / 2) + largeTextHeight * 2;

    dark_mode ? tft.fillScreen(TFT_BLACK) : tft.fillScreen(TFT_WHITE);

    // Print the large text centered on the screen
    tft.setCursor(centerX, centerY);
    tft.println(A);
    tft.setCursor(centerX1, centerY1);
    tft.println(B);
    tft.setCursor(centerX2, centerY2);
    tft.println(C);
}
