// Emotion.h
#ifndef Emotion_h
#define Emotion_h

#include <Arduino.h>
#include <vector>
#include <functional>

class Emotion {
public:
    Emotion();
    void begin(uint8_t happiness, uint8_t tiredness, uint8_t anger, uint8_t sadness, uint8_t fear);
    void addActivity(uint8_t emotion, int points, std::function<void()> function);
    void updateEmotions(uint8_t happiness, uint8_t tiredness, uint8_t anger, uint8_t sadness, uint8_t fear);
    void handleActivities();
    void updateEmotion(uint8_t emotion, int delta);

private:
    uint8_t happiness;
    uint8_t tiredness;
    uint8_t anger;
    uint8_t sadness;
    uint8_t fear;
    std::vector<std::pair<uint8_t, std::pair<int, std::function<void()>>>> activities;
    std::vector<uint8_t> calculatePriority();
};

#endif

// Example code 1#

//Emotion emotion;
//
//void setupEmotions() {
//    emotion.begin(128, 64, 32, 16, 8); // Imposta i valori iniziali delle emozioni
//
//    // Aggiungi le attività
//    emotion.addActivity(0, 100, []() { /* Codice per gestire un'attività legata all'emozione "happiness" */ });
//    emotion.addActivity(1, 200, []() { /* Codice per gestire un'attività legata all'emozione "tiredness" */ });
//    emotion.addActivity(2, 50, []() { /* Codice per gestire un'attività legata all'emozione "anger" */ });
//    // Aggiungi altre attività...
//}
//
//void loop() {
//    // Aggiorna i valori delle emozioni
//    emotion.updateEmotions(128, 64, 32, 16, 8);
//
//    // Gestisci le attività in base alle priorità
//    emotion.handleActivities();
//
//    // Esegui un'attività "obbligatoria" direttamente
//    doMandatoryActivity();
//
//    // Modifica il valore di un'emozione in base a un'attività svolta
//    emotion.updateEmotion(0, 10); // Incrementa "happiness" di 10 punti
//
//    // Altre operazioni di loop...
//}
//
//void doMandatoryActivity() {
//    // Codice per eseguire un'attività "obbligatoria"
//    // ...
//}


// Example code 2#

//Emotion emotion;
//
//// Funzione per l'attività "Gioca"
//void playActivity() {
//    // Codice per l'attività "Gioca"
//    // ...
//    emotion.updateEmotion(0, 20);  // Aumenta la felicità di 20 punti
//    emotion.updateEmotion(1, -10); // Diminuisce la stanchezza di 10 punti
//}
//
//// Funzione per l'attività "Dormi"
//void sleepActivity() {
//    // Codice per l'attività "Dormi"
//    // ...
//    emotion.updateEmotion(1, -50); // Diminuisce la stanchezza di 50 punti
//    emotion.updateEmotion(3, -15); // Diminuisce la tristezza di 15 punti
//}
//
//// Funzione per l'attività "Mangia"
//void eatActivity() {
//    // Codice per l'attività "Mangia"
//    // ...
//    emotion.updateEmotion(0, 10);  // Aumenta la felicità di 10 punti
//    emotion.updateEmotion(2, -20); // Diminuisce la rabbia di 20 punti
//}
//
//void setup() {
//    emotion.begin(100, 50, 30, 70, 20);
//    emotion.addActivity(0, 80, playActivity);  // Attività "Gioca" associata alla felicità con priorità 80
//    emotion.addActivity(1, 120, sleepActivity);  // Attività "Dormi" associata alla stanchezza con priorità 120
//    emotion.addActivity(2, 90, eatActivity);  // Attività "Mangia" associata alla rabbia con priorità 90
//}
//
//void loop() {
//    // ...
//    emotion.handleActivities();
//    // ...
//}