// LLMManager.cpp
// Inclusione del file di intestazione per la classe LLMManager
#include "LLMManager.h"

// Inclusione dei file di intestazione per le sottoclassi specifiche delle API LLM
#include "OpenAIApi.h"
#include "AnthropicApi.h"
#include "HuggingFaceApi.h"

// Definizione del costruttore della classe LLMManager
LLMManager::LLMManager() {
  // Inizializzazione dell'indice dell'API attiva a -1
  currentApiIndex = -1;

  // Aggiunta di un'istanza di OpenAIApi al vettore llmApis
  llmApis.push_back(new OpenAIApi("gpt-3.5-turbo", "https://api.openai.com/v1/chat/completions", "YOUR_OPENAI_API_KEY", "gpt-3.5-turbo", "You are a helpful AI assistant."));

  // Aggiunta di istanze di AnthropicApi al vettore llmApis
  llmApis.push_back(new AnthropicApi("claude-3-opus", "https://api.anthropic.com/v1/messages", "YOUR_ANTHROPIC_API_KEY", "claude-3-opus-20240229", "You are a helpful AI assistant."));
  llmApis.push_back(new AnthropicApi("claude-3-sonnet", "https://api.anthropic.com/v1/messages", "YOUR_ANTHROPIC_API_KEY", "claude-3-sonnet-20240229", "You are a helpful AI assistant."));
  llmApis.push_back(new AnthropicApi("claude-3-haiku", "https://api.anthropic.com/v1/messages", "YOUR_ANTHROPIC_API_KEY", "claude-3-haiku-20240307", "You are a helpful AI assistant."));

  // Aggiunta di istanze di HuggingFaceApi al vettore llmApis
  llmApis.push_back(new HuggingFaceApi("gemma-1.1-7b-it", "https://api-inference.huggingface.co/models", "YOUR_HUGGINGFACE_API_KEY", "/google/gemma-1.1-7b-it", "You are a helpful AI assistant."));
  llmApis.push_back(new HuggingFaceApi("Mistral-7B-Instruct-v0.2", "https://api-inference.huggingface.co/models", "YOUR_HUGGINGFACE_API_KEY", "/mistralai/Mistral-7B-Instruct-v0.2", "You are a helpful AI assistant."));
  llmApis.push_back(new HuggingFaceApi("Mixtral-8x7B-Instruct-v0.1", "https://api-inference.huggingface.co/models", "YOUR_HUGGINGFACE_API_KEY", "/mistralai/Mixtral-8x7B-Instruct-v0.1", "You are a helpful AI assistant."));
}

// Definizione del metodo begin() della classe LLMManager
void LLMManager::begin(String apiName = "") {
  // Creazione di un'istanza di WiFiManager per gestire la connessione Wi-Fi
  WiFiManager wifiManager;

  // Avvio della connessione Wi-Fi utilizzando autoConnect() con il nome della rete "LLMManager"
  wifiManager.autoConnect("LLMManager");

  // Inizializzazione dell'indice dell'API attiva a -1
  currentApiIndex = -1;

  if (apiName != "") {
    // Ricerca dell'API corrispondente al nome specificato
    Serial.println("cerca api dal nome");
    for (int i = 0; i < llmApis.size(); i++) {
      if (llmApis[i]->getApiName() == apiName) {
        currentApiIndex = i;
        break;
      }
    }

    // Verifica se l'API specificata è stata trovata
    if (currentApiIndex == -1) {
      Serial.print("Error: API '");
      Serial.print(apiName);
      Serial.println("' not found.");

      Serial.print("cerca api funzionante");
      // Iterazione attraverso il vettore llmApis per verificare se ciascuna API è attiva
      for (int i = 0; i < llmApis.size(); i++) {

        Serial.println("LLM selected: ");
        Serial.print(i);
        Serial.println("");

        // Invio di una richiesta di test all'API corrente
        if (llmApis[i]->checkApi()) {
          // Se l'API è attiva, impostazione dell'indice dell'API attiva e interruzione del ciclo
          currentApiIndex = i;
          break;
        }
      }
    }
  } else {
    Serial.print("cerca api funzionante");
    // Iterazione attraverso il vettore llmApis per verificare se ciascuna API è attiva
    for (int i = 0; i < llmApis.size(); i++) {

      Serial.println("LLM selected: ");
      Serial.print(i);
      Serial.println("");

      // Invio di una richiesta di test all'API corrente
      if (llmApis[i]->checkApi()) {
        // Se l'API è attiva, impostazione dell'indice dell'API attiva e interruzione del ciclo
        currentApiIndex = i;
        break;
      }
    }
  }
}

// Definizione del metodo generateResponse() della classe LLMManager
String LLMManager::generateResponse(String input) {
  // Verifica se non è stata trovata alcuna API attiva
  if (currentApiIndex == -1) {
    // Restituzione di un messaggio di errore se non è stata trovata alcuna API attiva
    return "Error: No active LLM API found.";
  }

  // Chiamata del metodo generateResponse() della sottoclasse API corrente per generare una risposta
  return llmApis[currentApiIndex]->generateResponse(input);
}