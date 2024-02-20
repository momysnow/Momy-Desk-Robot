from flask import Blueprint, jsonify

import requests

weather_bp = Blueprint('weather', __name__)


class Weather:
    def __init__(self, apiKey):
        self._apiKey = apiKey

    def get_coordinates(self):
        response = requests.get("http://ipinfo.io/json")
        if response.status_code == 200:
            data = response.json()
            coordinates = data["loc"]
            return coordinates
        else:
            print("Error on HTTP request")
            return ""

    def get_weather(self, units):
        data = {}
        units_param = ""

        if units == 0:
            units_param = "standard"
        elif units == 1:
            units_param = "metric"
        elif units == 2:
            units_param = "imperial"
        else:
            print("Invalid units parameter")
            return data

        coordinates = self.get_coordinates()

        if coordinates == "":
            print("Error getting coordinates")
            return data

        lat, lon = coordinates.split(',')
        url = f"http://api.openweathermap.org/data/2.5/weather?lat={lat}&lon={lon}&appid={self._apiKey}&units={units_param}"
        print(url)

        response = requests.get(url)

        if response.status_code == 200:
            data = response.json()
            weather_data = {
                "temperature": data["main"]["temp"],
                "units": units,
                "weatherMain": data["weather"][0]["main"],
                "weatherIcon": data["weather"][0]["icon"]
            }
            return weather_data
        else:
            print("Error on HTTP request")
            return data


weather = Weather("7ddc20023cdd9038830ba467ab3fcd99")


@weather_bp.route('/weather/get_temperature', methods=['GET'])
def get_temperature():
    # Esempio di utilizzo
    weather_data = weather.get_weather(1)

    # Qui dovresti sostituire questa logica con il codice effettivo per ottenere la temperatura in tempo reale
    temperature = str(round(weather_data["temperature"], 0)) + "°"
    return jsonify({'temp': temperature})
