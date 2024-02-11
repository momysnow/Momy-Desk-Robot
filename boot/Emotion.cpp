#include "Emotion.h"
#include <math.h>

Emotion::Emotion() {
}

void Emotion::begin(uint8_t happiness, uint8_t tiredness, uint8_t anger, uint8_t sadness, uint8_t fear) {
  this->happiness = happiness;
  this->tiredness = tiredness;
  this->anger = anger;
  this->sadness = sadness;
  this->fear = fear;
}

std::vector<uint8_t> Emotion::calculatePriority() {
  // Calcolo delle priorità in punteggi
  int score_happiness = 100 - (static_cast<int>(happiness) * 10) 1 / 255 * (static_cast<int>(tiredness)*static_cast<int>(tiredness));
  int score_tiredness = 1 / 255 * (static_cast<int>(tiredness)*static_cast<int>(tiredness));
  int score_anger = static_cast<int>(anger);
  int score_sadness = static_cast<int>(sadness);
  int score_fear = static_cast<int>(fear);

  // Creiamo un vettore di numeri corrispondenti alle emozioni in ordine di priorità
  std::vector<int> priorityList = { score_happiness, score_tiredness, score_anger, score_sadness, score_fear };

  // Ordiniamo il vettore in base alle priorità calcolate
  std::sort(priorityList.begin(), priorityList.end(), std::greater<int>());

  // Creiamo un vettore contenente solo i numeri corrispondenti alle emozioni in ordine di priorità
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
