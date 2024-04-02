// Emotion.cpp
#include "Emotion.h"
#include <algorithm>
#include <functional>

Emotion::Emotion() {}

void Emotion::begin(uint8_t happiness, uint8_t tiredness, uint8_t anger, uint8_t sadness, uint8_t fear) {
    this->happiness = happiness;
    this->tiredness = tiredness;
    this->anger = anger;
    this->sadness = sadness;
    this->fear = fear;
}

void Emotion::addActivity(uint8_t emotion, int points, std::function<void()> function) {
    activities.emplace_back(emotion, std::make_pair(points, function));
}

void Emotion::updateEmotions(uint8_t happiness, uint8_t tiredness, uint8_t anger, uint8_t sadness, uint8_t fear) {
    this->happiness = happiness;
    this->tiredness = tiredness;
    this->anger = anger;
    this->sadness = sadness;
    this->fear = fear;
}

void Emotion::updateEmotion(uint8_t emotion, int delta) {
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

void Emotion::handleActivities() {
    std::vector<uint8_t> priorities = calculatePriority();

    // Esegui le attività in base alle priorità
    for (uint8_t priority : priorities) {
        for (auto& activity : activities) {
            if (activity.first == priority) {
                activity.second.second();
                break;
            }
        }
    }
}

std::vector<uint8_t> Emotion::calculatePriority() {
    // Calcolo delle priorità in punteggi
    // più il punteggio è basso maggiore è la necessità

    // Funzioni di normalizzazione
    auto easeInOutSine = [](double x) { return -(cos(M_PI * x) - 1) / 2; };
    auto easeInOutCubic = [](double x) { return x < 0.5 ? 4 * pow(x, 3) : 1 - pow(-2 * x + 2, 3) / 2; };
    auto easeInQuint = [](double x) { return pow(x, 5); };
    auto easeInOutBack = [](double x) {
        const double c1 = 1.70158;
        const double c2 = c1 * 1.525;
        return x < 0.5 ? (pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
                       : (pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
    };
    auto easeInOutBounce = [](double x) {
        return x < 0.5 ? (1 - easeOutBounce(1 - 2 * x)) / 2 : (1 + easeOutBounce(2 * x - 1)) / 2;
    };

    int score_happiness = static_cast<int>(255 * easeInOutCubic(1 - (happiness / 255.0)));
    int score_tiredness = static_cast<int>(255 * easeInOutSine(tiredness / 255.0));
    int score_anger = static_cast<int>(255 * easeInQuint(anger / 255.0));
    int score_sadness = static_cast<int>(255 * easeInOutBack(sadness / 255.0));
    int score_fear = static_cast<int>(255 * easeInOutBounce(fear / 255.0));

    // Creiamo un vettore di numeri corrispondenti alle emozioni in ordine di priorità
    std::vector<int> priorityList = {score_happiness, score_tiredness, score_anger, score_sadness, score_fear};

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