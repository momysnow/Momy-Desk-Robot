// HuggingFaceApi.cpp
#include "HuggingFaceApi.h"

HuggingFaceApi::HuggingFaceApi(String apiName, String baseUrl, String apiKey, String modelPath, String systemPrompt) {
  this->apiName = apiName;
  this->baseUrl = baseUrl;
  this->apiKey = apiKey;
  this->modelPath = modelPath;
  this->systemPrompt = systemPrompt;
}

// Implementazioni sovrascritte dei metodi, se necessario

String HuggingFaceApi::formatRequest(String input) {
  return "Assistant: " + systemPrompt + "\ User: " + input + "\ Assistant: ";
}

bool HuggingFaceApi::checkApi() {
    bool validate = false;
    String response = sendRequest("hi");

    // Test
    Serial.println("HuggingFaceApi: ");
    Serial.println(response);
    // Test

    if (response == "") {
        Serial.println("Error: Empty response");
        return false;
    }

    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, response);

    if (error) {
        Serial.print("Error parsing JSON: ");
        Serial.println(error.c_str());
        return false;
    }

    if (doc.containsKey("error")) {
        const char* errorMessage = doc["error"]["message"];
        Serial.print("Error: ");
        Serial.println(errorMessage);
        validate = false;
    } else {
        validate = true;
    }

    return validate;
}