// LLMApiBase.cpp
#include "LLMApiBase.h"

String LLMApiBase::sendRequest(String input) {
  HTTPClient http;
  String response;
  http.begin(baseUrl + modelPath);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", "Bearer " + apiKey);
  String data = "{\"inputs\": \"" + input + "\"}";
  int httpResponseCode = http.POST(data);

  if (httpResponseCode > 0) {
    response = http.getString();
  } else {
    response = "Error: " + String(httpResponseCode);
  }

  http.end();
  return response;
}

String LLMApiBase::generateResponse(String input) {
  String formattedRequest = formatRequest(input);  // Utilizzo di formatRequest
  String sanitizedInput = sanitizeInput(formattedRequest);  // Rimuove i caratteri non stampabili
  return sendRequest(formattedRequest);
}

String LLMApiBase::formatRequest(String input) {
  return "Assistant: " + systemPrompt + " \n User: " + input + " \n Assistant: ";
}

String LLMApiBase::sanitizeInput(String input) {
  String sanitized = "";
  for (int i = 0; i < input.length(); i++) {
    char c = input.charAt(i);
    // Solo aggiungere caratteri stampabili
    if (c >= 32) {
      sanitized += c;
    }
  }
  return sanitized;
}

// Nelle sottoclassi (esempio per OpenAIApi)
String LLMApiBase::getApiName() {
  return apiName;
}