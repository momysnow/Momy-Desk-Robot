#include "LLMManager.h"

LLMManager::LLMManager() {
    currentApiIndex = -1;

    // Aggiungi API di LLM predefinite
    // openai LLM
    addLLMApi("https://api.openai.com/v1/chat/completions", "YOUR_OPENAI_API_KEY", "gpt-3.5-turbo", "You are a helpful AI assistant.", false);

    // anthropic LLM
    addLLMApi("https://api.anthropic.com/v1/complete", "YOUR_ANTHROPIC_API_KEY", "claude-3-opus-20240229", "You are a helpful AI assistant.", false);
    addLLMApi("https://api.anthropic.com/v1/complete", "YOUR_ANTHROPIC_API_KEY", "claude-3-sonnet-20240229", "You are a helpful AI assistant.", false);
    addLLMApi("https://api.anthropic.com/v1/complete", "YOUR_ANTHROPIC_API_KEY", "claude-3-haiku-20240307", "You are a helpful AI assistant.", false);

    // huggingface LLM
    addLLMApi("https://api-inference.huggingface.co/models", "YOUR_HUGGINGFACE_API_KEY", "/google/gemma-7b-it", "You are a helpful AI assistant.", false);
    addLLMApi("https://api-inference.huggingface.co/models", "YOUR_HUGGINGFACE_API_KEY", "/google/gemma-2b-it", "You are a helpful AI assistant.", false);
    addLLMApi("https://api-inference.huggingface.co/models", "YOUR_HUGGINGFACE_API_KEY", "/mistralai/Mixtral-8x7B-Instruct-v0.1", "You are a helpful AI assistant.", true);

    // huggingface LLM speech recognition
    //https://huggingface.co/openai/whisper-tiny

    // huggingface LLM action
    //https://huggingface.co/facebook/bart-large-mnli

    // huggingface LLM question answer
    //https://huggingface.co/deepset/roberta-base-squad2
}

void LLMManager::begin() {
    WiFiManager wifiManager;
    wifiManager.autoConnect("LLMManager");

    currentApiIndex = -1;
    for (int i = 0; i < llmApis.size(); i++) {
        if (llmApis[i].active) {
            currentApiIndex = i;
            break;
        }
    }
}

void LLMManager::addLLMApi(String baseUrl, String apiKey, String modelPath, String systemPrompt, bool active) {
    LLMApi newApi = {baseUrl, apiKey, modelPath, systemPrompt, active};
    llmApis.push_back(newApi);
}

String LLMManager::generateResponse(String input) {
    return sendRequest(input);
}

String LLMManager::sendRequest(String input) {
    if (currentApiIndex == -1) {
        return "Error: No active LLM API found.";
    }

    HTTPClient http;
    String response;
    http.begin(llmApis[currentApiIndex].baseUrl + llmApis[currentApiIndex].modelPath);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", "Bearer " + llmApis[currentApiIndex].apiKey);
    String data = "{\"inputs\": \"" + llmApis[currentApiIndex].systemPrompt + input + "\"}";
    int httpResponseCode = http.POST(data);

    if (httpResponseCode > 0) {
        response = http.getString();
    } else {
        response = "Error: " + String(httpResponseCode);
    }

    http.end();
    return response;
}

bool LLMManager::verifyLLMApi(LLMApi& api) {
    HTTPClient http;
    http.begin(api.baseUrl + api.modelPath);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", "Bearer " + api.apiKey);
    String data = "{\"inputs\": \"" + api.systemPrompt + "Hello\"}";
    int httpResponseCode = http.POST(data);
    http.end();

    if (httpResponseCode == 200) {
        return true; // API funzionante
    } else {
        return false; // API non funzionante
    }
}