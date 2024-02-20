from flask import Blueprint

import serial.tools.list_ports
import json

serial_bp = Blueprint('serial', __name__)


@serial_bp.route('/serial/get_available_ports', methods=['GET'])
# Funzione per ottenere le porte disponibili
def get_available_ports():
    # Utilizza la libreria pyserial per ottenere una lista delle porte seriali disponibili
    available_ports = serial.tools.list_ports.comports()
    # Crea un dizionario che mappa il nome della porta al nome della porta stesso
    port_info = {port.device: port.description for port in available_ports}
    print(port_info)
    # Restituisci il dizionario come JSON
    return json.dumps(port_info)