#ifndef Emotion_h
#define Emotion_h

#include <Arduino.h>
#include <vector>

struct Activity {
  uint8_t emotion;
  int points;
  void (*function)();
};

class Emotion {
public:
  Emotion();
  void begin(uint8_t happiness, uint8_t tiredness, uint8_t anger, uint8_t sadness, uint8_t fear);
  void addActivity(uint8_t emotion, int points, void (*function)());
  std::vector<uint8_t> calculatePriority();

private:
  uint8_t happiness;
  uint8_t tiredness;
  uint8_t anger;
  uint8_t sadness;
  uint8_t fear;
  std::vector<Activity> activityList;
};

#endif