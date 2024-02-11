#include "ChatGPT.h"

ChatGPT::ChatGPT() {}

void ChatGPT::begin() {
    WiFiManager wifiManager;
    wifiManager.autoConnect("ChatGPT");
}

void ChatGPT::setApiKey(String apiKey) {
    this->apiKey = apiKey;
}

void ChatGPT::setModel(String modelName) {
    this->selectedModel = modelName;
}

String ChatGPT::generateResponse(String input) {
    return sendRequest(input);
}

String ChatGPT::sendRequest(String input) {
    HTTPClient http;
    String response;

    http.begin("http://api.openai.com/v1/chat/completions");
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", "Bearer " + apiKey);

    String data = "{\"model\": \"" + selectedModel + "\", \"prompt\": \"" + input + "\"}";

    int httpResponseCode = http.POST(data);
    if (httpResponseCode > 0) {
        response = http.getString();
    } else {
        response = "Error: " + String(httpResponseCode);
    }

    http.end();
    return response;
}
