// Emotions.h
#ifndef Emotions_h
#define Emotions_h

#include <Arduino.h>
#include <vector>
#include <functional>
#include <cmath>

/**
 * Emotions class represents a set of emotions with associated values and activities.
 */
class Emotions {
public:
    /**
     * Initializes the emotions with default values.
     */
    Emotions();

    /**
     * Begins the emotions with the specified values.
     * @param happiness Initial happiness value (0-255)
     * @param tiredness Initial tiredness value (0-255)
     * @param anger Initial anger value (0-255)
     * @param sadness Initial sadness value (0-255)
     * @param fear Initial fear value (0-255)
     */
    void begin(uint8_t happiness, uint8_t tiredness, uint8_t anger, uint8_t sadness, uint8_t fear);

    /**
     * Adds an activity with the specified emotion, points, and function.
     * @param emotion Emotion type (0-4)
     * @param points Points associated with the activity
     * @param function Function to be executed when the activity is handled
     */
    void addActivity(uint8_t emotion, int points, std::function<void()> function);

    /**
     * Updates the emotions with the specified values.
     * @param happiness New happiness value (0-255)
     * @param tiredness New tiredness value (0-255)
     * @param anger New anger value (0-255)
     * @param sadness New sadness value (0-255)
     * @param fear New fear value (0-255)
     */
    void updateEmotions(uint8_t happiness, uint8_t tiredness, uint8_t anger, uint8_t sadness, uint8_t fear);

    /**
     * Updates a specific emotion with the specified delta value.
     * @param emotion Emotion type (0-4)
     * @param delta Delta value to be added to the emotion
     */
    void updateEmotion(uint8_t emotion, int delta);

    /**
     * Handles the activities based on their priorities.
     */
    void handleActivities();

    /**
     * Gets the current happiness value.
     * @return Happiness value (0-255)
     */
    uint8_t getHappiness() { return happiness; }

    /**
     * Gets the current tiredness value.
     * @return Tiredness value (0-255)
     */
    uint8_t getTiredness() { return tiredness; }

    /**
     * Gets the current anger value.
     * @return Anger value (0-255)
     */
    uint8_t getAnger() { return anger; }

    /**
     * Gets the current sadness value.
     * @return Sadness value (0-255)
     */
    uint8_t getSadness() { return sadness; }

    /**
     * Gets the current fear value.
     * @return Fear value (0-255)
     */
    uint8_t getFear() { return fear; }

private:
    uint8_t happiness;
    uint8_t tiredness;
    uint8_t anger;
    uint8_t sadness;
    uint8_t fear;
    std::vector<std::pair<uint8_t, std::pair<int, std::function<void()>>>> activities;

    /**
     * Calculates the priorities of the emotions.
     * @return A vector of emotion types in priority order
     */
    std::vector<uint8_t> calculatePriority();
};

#endif