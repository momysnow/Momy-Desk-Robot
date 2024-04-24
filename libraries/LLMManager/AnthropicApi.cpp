// AnthropicApi.cpp
#include "AnthropicApi.h"

AnthropicApi::AnthropicApi(String apiName, String baseUrl, String apiKey, String modelPath, String systemPrompt) {
  this->apiName = apiName;
  this->baseUrl = baseUrl;
  this->apiKey = apiKey;
  this->modelPath = modelPath;
  this->systemPrompt = systemPrompt;
}

String AnthropicApi::sendRequest(String input) {
  HTTPClient http;
  String response;
  http.begin(baseUrl);
  http.addHeader("x-api-key", apiKey);
  http.addHeader("anthropic-version", "2023-06-01");
  http.addHeader("Content-Type", "application/json");
  String data = "{\"model\": \"" + modelPath + "\", \"max_tokens\": 1024, \"messages\": [{\"role\": \"user\", \"content\": \"" + input + "\"}]}";
  int httpResponseCode = http.POST(data);

  if (httpResponseCode > 0) {
    String jsonResponse = http.getString();

    // Parsing della risposta JSON utilizzando ArduinoJson
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, jsonResponse);

    if (error) {
      response = "Error parsing JSON: " + String(error.c_str());
    } else {
      // Estrazione del testo di risposta
      JsonArray contentArray = doc["content"];
      if (!contentArray.isNull() && contentArray.size() > 0) {
        JsonObject contentObject = contentArray[0];
        if (contentObject.containsKey("text")) {
          response = contentObject["text"].as<String>();
        } else {
          response = "Error: Text not found in response";
        }
      } else {
        response = "Error: Content array is empty or null";
      }
    }
  } else {
    response = "Error: " + String(httpResponseCode);
  }

  http.end();
  return response;
}

String AnthropicApi::formatRequest(String input) {
  return input;
}

bool AnthropicApi::checkApi() {
  bool validate = false;
  String response = sendRequest("hi");

  // Test
  Serial.println("AnthropicApi: ");
  Serial.println(response);
  // Test

  if (response == "") {
    Serial.println("Error: Empty response");
    validate = false;
  } else if (response.startsWith("Error")) {
    Serial.println(response);
    validate = false;
  } else {
    // Prova a fare il parsing del JSON solo se la risposta non inizia con "Error"
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, response);

    if (error) {
      // Se il parsing JSON fallisce, considera la risposta come testo semplice
      validate = true;
    } else {
      // Se il parsing JSON ha successo, verifica se c'è un errore nel JSON
      if (doc.containsKey("error")) {
        const char* errorMessage = doc["error"]["message"];
        Serial.print("Error: ");
        Serial.println(errorMessage);
        validate = false;
      } else {
        validate = true;
      }
    }
  }

  return validate;
}
