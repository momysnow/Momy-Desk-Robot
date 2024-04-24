// LLMManager.ino
#include <Arduino.h>
#include "LLMManager.h"

LLMManager llmManager;

void setup() {
  Serial.begin(115200);

  Serial.println("Initializing LLMManager...");
  llmManager.begin("");
  Serial.println("LLMManager initialized.");
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.length() > 0) {
      Serial.print("Generating response for input: ");
      Serial.println(input);

      String response = llmManager.generateResponse(input);

      Serial.print("Generated response: ");
      Serial.println(response);
    }
  }
}