// HuggingFaceApi.h
#ifndef HUGGINGFACE_API_H
#define HUGGINGFACE_API_H

#include "LLMApiBase.h"

// Sottoclasse concreta per l'API di Hugging Face
class HuggingFaceApi : public LLMApiBase {
public:
  HuggingFaceApi(String apiName, String baseUrl, String apiKey, String modelPath, String systemPrompt);
  // Sovrascrivere i metodi se necessario
  // String sendRequest(String input) override;
  // String generateResponse(String input) override;
  String formatRequest(String input) override;
  // String sanitizeInput(String input) override;
  // String getApiName() override;
  bool checkApi() override;
};

#endif