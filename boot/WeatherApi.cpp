#include "WeatherApi.h"

Weather::Weather(String apiKey) {
  _apiKey = apiKey;
}

String Weather::getCoordinates() {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, "http://ipinfo.io/json");
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    String payload = http.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);

    String coordinates = doc["loc"];
    return coordinates;
  } else {
    Serial.println("Error on HTTP request");
    return "";
  }

  http.end();
}

WeatherData Weather::getWeather(int units) {
  WeatherData data;
  String unitsParam;
  
  switch (units) {
    case 0:
      unitsParam = "standard";
      break;
    case 1:
      unitsParam = "metric";
      break;
    case 2:
      unitsParam = "imperial";
      break;
    default:
      Serial.println("Invalid units parameter");
      return data;
  }
  
  String coordinates = getCoordinates();
  
  if (coordinates == "") {
    Serial.println("Error getting coordinates");
    return data;
  }

  WiFiClient client;
  HTTPClient http;

  // Parse coordinates
  int commaIndex = coordinates.indexOf(',');
  String lat = coordinates.substring(0, commaIndex);
  String lon = coordinates.substring(commaIndex + 1);

  String url = "http://api.openweathermap.org/data/2.5/weather?lat=" + lat + "&lon=" + lon + "&appid=" + _apiKey + "&units=" + unitsParam;
  Serial.println(url);
  
  http.begin(client, url);
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    String payload = http.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);

    data.temperature = doc["main"]["temp"].as<float>();
    data.units = units;
    data.weatherMain = doc["weather"][0]["main"].as<String>();
    data.weatherIcon = doc["weather"][0]["icon"].as<String>();

    return data;
  } else {
    Serial.println("Error on HTTP request");
    return data;
  }

  http.end();
}
