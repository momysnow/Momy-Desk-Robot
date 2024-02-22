#ifndef CHATGPT_H
#define CHATGPT_H

#include <Arduino.h>
#include <WiFiManager.h>
#include <HTTPClient.h>

class ChatGPT {
public:
  ChatGPT();
  void begin();
  String generateResponse(String input);
  void setApiKey(String apiKey);
  void setModel(String modelName);
private:
  String apiKey;
  String selectedModel;
  String sendRequest(String input);
};
#endif


//Example
/*
#include <Arduino.h>
#include <chatgpt.h>

ChatGPT chat;

// Definizione delle costanti per i nomi dei modelli
const String MODEL_DAVINCI = "text-davinci-003";
const String MODEL_CURIE = "text-curie-003";

// Impostazione della chiave API e del modello desiderato
const String OPENAI_API_KEY = "YOUR_OPENAI_API_KEY";

void setup() {
    Serial.begin(115200);
    chat.begin();
    chat.setApiKey(OPENAI_API_KEY);
    chat.setModel(MODEL_DAVINCI); // Puoi cambiare il modello qui
}

void loop() {
    String userInput;
    Serial.println("Enter your message:");
    while (Serial.available() == 0) {
        delay(100);
    }
    userInput = Serial.readString();
    String response = chat.generateResponse(userInput);
    Serial.println("Response from ChatGPT:");
    Serial.println(response);
}
*/