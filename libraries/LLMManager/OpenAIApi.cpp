// OpenAIApi.cpp
#include "OpenAIApi.h"

OpenAIApi::OpenAIApi(String apiName, String baseUrl, String apiKey, String modelPath, String systemPrompt) {
  this->apiName = apiName;
  this->baseUrl = baseUrl;
  this->apiKey = apiKey;
  this->modelPath = modelPath;
  this->systemPrompt = systemPrompt;
}

bool OpenAIApi::checkApi() {
    bool validate = false;

    String response = sendRequest("hi");
    
    if (response == "") {
        Serial.println("Error: Empty response");
        return false;
    }

    // Utilizza ArduinoJson per analizzare il JSON
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, response);

    if (error) {
        Serial.print("Error parsing JSON: ");
        Serial.println(error.c_str());
        return false;
    }

    // Verifica se la risposta contiene un errore
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