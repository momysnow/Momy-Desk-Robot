// Emotions.cpp
#include "Emotions.h"
#include <algorithm>

// Default constructor
Emotions::Emotions() {}

// Initializes the emotions with the specified values
void Emotions::begin(uint8_t happiness, uint8_t tiredness, uint8_t anger, uint8_t sadness, uint8_t fear) {
  this->happiness = happiness;
  this->tiredness = tiredness;
  this->anger = anger;
  this->sadness = sadness;
  this->fear = fear;
}

// Adds an activity to the activities vector
// Each activity is associated with an emotion type, points, and a function to be executed
void Emotions::addActivity(uint8_t emotion, int points, std::function<void()> function) {
  activities.push_back(std::make_pair(emotion, std::make_pair(points, function)));
}

// Updates the emotions with the specified values
void Emotions::updateEmotions(uint8_t happiness, uint8_t tiredness, uint8_t anger, uint8_t sadness, uint8_t fear) {
  this->happiness = happiness;
  this->tiredness = tiredness;
  this->anger = anger;
  this->sadness = sadness;
  this->fear = fear;
}

// Updates a specific emotion by adding the delta value
// The emotion value is constrained between 0 and 255
void Emotions::updateEmotion(uint8_t emotion, int delta) {
  switch (emotion) {
    case 0:
      happiness = constrain(happiness + delta, 0, 255);
      break;
    case 1:
      tiredness = constrain(tiredness + delta, 0, 255);
      break;
    case 2:
      anger = constrain(anger + delta, 0, 255);
      break;
    case 3:
      sadness = constrain(sadness + delta, 0, 255);
      break;
    case 4:
      fear = constrain(fear + delta, 0, 255);
      break;
  }
}

// Handles the activities based on their priorities
void Emotions::handleActivities() {
  // Calculate the priority order of emotions
  std::vector<uint8_t> priorities = calculatePriority();

  // Execute activities in priority order
  for (uint8_t priority : priorities) {
    for (auto& activity : activities) {
      // If the activity matches the current priority, execute its function and break out of the inner loop
      if (activity.first == priority) {
        activity.second.second();
        break;
      }
    }
  }
}

// Calculates the priorities of the emotions based on their current values
// The priorities are calculated using easing functions
// Lower score means higher priority
std::vector<uint8_t> Emotions::calculatePriority() {
  // Define easing functions for priority calculation
  auto easeInOutSine = [](double x) {
    return -(cos(M_PI * x) - 1) / 2;
  };
  auto easeInOutCubic = [](double x) {
    return x < 0.5 ? 4 * pow(x, 3) : 1 - pow(-2 * x + 2, 3) / 2;
  };
  auto easeInQuint = [](double x) {
    return pow(x, 5);
  };
  auto easeInOutBack = [](double x) {
    const double c1 = 1.70158;
    const double c2 = c1 * 1.525;
    return x < 0.5 ? (pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
                   : (pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
  };
  auto easeOutBounce = [](double x) {
    const double n1 = 7.5625;
    const double d1 = 2.75;
    if (x < 1 / d1) {
      return n1 * x * x;
    } else if (x < 2 / d1) {
      return n1 * (x -= 1.5 / d1) * x + 0.75;
    } else if (x < 2.5 / d1) {
      return n1 * (x -= 2.25 / d1) * x + 0.9375;
    } else {
      return n1 * (x -= 2.625 / d1) * x + 0.984375;
    }
  };
  auto easeInOutBounce = [easeOutBounce](double x) {
    return x < 0.5 ? (1 - easeOutBounce(1 - 2 * x)) / 2 : (1 + easeOutBounce(2 * x - 1)) / 2;
  };

  // Calculate the scores for each emotion using the easing functions
  int score_happiness = static_cast<int>(255 * easeInOutCubic(1 - (happiness / 255.0)));
  int score_tiredness = static_cast<int>(255 * easeInOutSine(tiredness / 255.0));
  int score_anger = static_cast<int>(255 * easeInQuint(anger / 255.0));
  int score_sadness = static_cast<int>(255 * easeInOutBack(sadness / 255.0));
  int score_fear = static_cast<int>(255 * easeInOutBounce(fear / 255.0));

  // Create a vector of scores in priority order (higher score means lower priority)
  std::vector<int> priorityList = { score_happiness, score_tiredness, score_anger, score_sadness, score_fear };
  std::sort(priorityList.begin(), priorityList.end(), std::greater<int>());

  // Create a vector of emotion types in priority order
  std::vector<uint8_t> result;
  for (const auto& score : priorityList) {
    if (score == score_happiness)
      result.push_back(0);
    else if (score == score_tiredness)
      result.push_back(1);
    else if (score == score_anger)
      result.push_back(2);
    else if (score == score_sadness)
      result.push_back(3);
    else if (score == score_fear)
      result.push_back(4);
  }

  return result;
}