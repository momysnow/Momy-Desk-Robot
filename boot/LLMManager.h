#ifndef LLM_MANAGER_H
#define LLM_MANAGER_H

#include <Arduino.h>
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <vector>

struct LLMApi {
    String baseUrl;
    String apiKey;
    String modelPath;
    String systemPrompt;
    bool active;
};

class LLMManager {
public:
    LLMManager();
    void begin();
    String generateResponse(String input);
    void addLLMApi(String baseUrl, String apiKey, String modelPath, String systemPrompt, bool active);

private:
    std::vector<LLMApi> llmApis;
    int currentApiIndex;
    String sendRequest(String input);
    bool verifyLLMApi(LLMApi& api);
};

#endif