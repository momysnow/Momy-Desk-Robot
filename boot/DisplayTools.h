#ifndef DISPLAY_TOOLS_H
#define DISPLAY_TOOLS_H

#include "animation.h"

class DisplayTools {
public:
  DisplayTools();
  void begin();
  void clear();
  void stop();

  void showLogo();

  void setupLargeText();
  void setupSmallText();
  void writeThreeLinesOfText(String A, String B, String C);
  void tearDownText();
};

// TODO: use the State object oriented pattern to decide what should be displayed

#endif