// AnthropicApi.h
#ifndef ANTHROPIC_API_H
#define ANTHROPIC_API_H

#include "LLMApiBase.h"

// Sottoclasse concreta per l'API di Anthropic
class AnthropicApi : public LLMApiBase {
public:
  AnthropicApi(String apiName, String baseUrl, String apiKey, String modelPath, String systemPrompt);
  // Sovrascrivere i metodi se necessario
  String sendRequest(String input) override;
  // String generateResponse(String input) override;
  String formatRequest(String input) override;
  // String sanitizeInput(String input) override;
  // String getApiName() override;
  bool checkApi() override;
};

#endif