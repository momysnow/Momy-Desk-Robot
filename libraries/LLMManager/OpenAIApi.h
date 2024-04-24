// OpenAIApi.h
#ifndef OPENAI_API_H
#define OPENAI_API_H

#include "LLMApiBase.h"

class OpenAIApi : public LLMApiBase {
public:
  OpenAIApi(String apiName, String baseUrl, String apiKey, String modelPath, String systemPrompt);
  // Sovrascrivere i metodi se necessario
  // String sendRequest(String input) override;
  // String generateResponse(String input) override;
  // String formatRequest(String input) override;
  // String sanitizeInput(String input) override;
  // String getApiName() override;
  bool checkApi() override;
};

#endif