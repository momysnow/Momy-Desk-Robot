#ifndef WeatherApi_h
#define WeatherApi_h

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

struct WeatherData {
  float temperature;
  int units;
  String weatherMain;
  String weatherIcon;
};

class Weather {
  public:
    Weather(String apiKey);
    WeatherData getWeather(int units); // 0 a "standard", 1 a "metric" e 2 a "imperial"
  private:
    String _apiKey;
    String getCoordinates();
};

#endif

/*Example
#include <WiFi.h>
#include "Weather.h"

const char* ssid = "NOME_DEL_TUO_NETWORK_WIFI";
const char* password = "PASSWORD_DEL_TUO_NETWORK_WIFI";
const char* apiKeyWeather = "TUO_API_KEY_OPENWEATHERMAP";

WiFiClient client;
Weather weather(apiKeyWeather);

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Connessione al Wi-Fi
  Serial.println();
  Serial.print("Connessione a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connessione Wi-Fi stabilita");
  Serial.println("Indirizzo IP: ");
  Serial.println(WiFi.localIP());

  // Ottenere e stampare i dati meteo
  WeatherData weatherData = weather.getWeather(1); // Utilizza le unità metriche
  Serial.print("Temperature: ");
  Serial.println(weatherData.temperature);
  Serial.print("Unit: ");
  Serial.println(weatherData.units);
  Serial.print("Weather Main: ");
  Serial.println(weatherData.weatherMain);
  Serial.print("Weather Icon: ");
  Serial.println(weatherData.weatherIcon);
}

void loop() {
  // Il loop è vuoto poiché non abbiamo bisogno di eseguire alcuna operazione continua
}
*/