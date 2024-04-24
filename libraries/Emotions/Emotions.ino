// Emotions.ino
#include "Emotions.h"

// Define some sample activity functions
void activity1() {
  Serial.println("Activity 1: Happiness increased!");
}

void activity2() {
  Serial.println("Activity 2: Tiredness decreased!");
}

void activity3() {
  Serial.println("Activity 3: Anger increased!");
}

Emotions emotions; // Create an instance of the Emotions class

void setup() {
  Serial.begin(115200);

  // Initialize emotions with some initial values
  emotions.begin(128, 64, 32, 16, 8);

  // Add some sample activities
  emotions.addActivity(0, 10, activity1); // Happiness activity
  emotions.addActivity(1, 20, activity2); // Tiredness activity
  emotions.addActivity(2, 30, activity3); // Anger activity

  // Print initial emotion values
  Serial.println("Initial emotions:");
  Serial.print("Happiness: "); Serial.println(emotions.getHappiness());
  Serial.print("Tiredness: "); Serial.println(emotions.getTiredness());
  Serial.print("Anger: "); Serial.println(emotions.getAnger());
  Serial.print("Sadness: "); Serial.println(emotions.getSadness());
  Serial.print("Fear: "); Serial.println(emotions.getFear());
}

void loop() {
  // Update some emotions
  emotions.updateEmotion(0, 10); // Increase happiness
  emotions.updateEmotion(1, -10); // Decrease tiredness
  emotions.updateEmotion(2, 20); // Increase anger

  // Handle activities based on priorities
  emotions.handleActivities();

  delay(1000); // Wait for a second
}