// LLMApiBase.h
#ifndef LLM_API_BASE_H
#define LLM_API_BASE_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

class LLMApiBase {
public:
  virtual String sendRequest(String input);
  virtual String generateResponse(String input);
  virtual String formatRequest(String input);
  virtual String sanitizeInput(String input);
  virtual String getApiName();
  virtual bool checkApi() = 0;

protected:
  String apiName;
  String baseUrl;
  String apiKey;
  String modelPath;
  String systemPrompt;
};

#endif