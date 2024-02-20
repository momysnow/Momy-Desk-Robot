import os
import json


# Funzione per ottenere lo stato dell'utente dal file JSON
def get_user_state():
    print(os.path.exists("db/user.json"))
    if os.path.exists("db/user.json"):
        with open("db/user.json", 'r') as file:
            return json.load(file)
    else:
        return {}


# Funzione per salvare lo stato dell'utente nel file JSON
def save_user_state(state):
    with open("db/user.json", 'w') as file:
        json.dump(state, file)


# Function to save port to JSON file
def save_port_to_json(port):
    with open('db/esp32.json', 'w') as file:
        json.dump({"port": port}, file)