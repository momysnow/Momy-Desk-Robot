// LLMManager.h
#ifndef LLM_MANAGER_H
#define LLM_MANAGER_H

#include <Arduino.h>
#include <WiFiManager.h>
#include <vector>

#include "LLMApiBase.h"

class LLMManager {
public:
  /**
   * Constructs a new LLMManager object.
   */
  LLMManager();

  /**
   * Initializes the LLMManager and establishes the Wi-Fi connection.
   */
  void begin(String apiName);

  /**
   * Generates a response using the active LLM API.
   * @param input The input string to generate a response for.
   * @return The generated response string.
   */
  String generateResponse(String input);

private:
  /**
   * Vector of pointers to objects derived from the LLMApiBase base class.
   */
  std::vector<LLMApiBase*> llmApis;

  /**
   * Index of the currently active LLM API.
   */
  int currentApiIndex;
};

#endif