from flask import Flask, render_template, request, url_for,redirect
from lib.driver import driver_bp
from lib.serial_communication import serial_bp
from plugin.weather import weather_bp
from lib.firmware import *
from lib.db import *

app = Flask(__name__)
app.register_blueprint(serial_bp)
app.register_blueprint(driver_bp)
app.register_blueprint(weather_bp)

HOST = '0.0.0.0'
PORT = 8000


# setup
@app.route('/')
def index():
    user_state = get_user_state()
    if 'visited' in user_state and user_state['visited']:
        return render_template('index.html')
    else:
        user_state['visited'] = True
        save_user_state(user_state)
        
        arduino = pyduinocli.Arduino("C:/Program Files/Arduino IDE/resources/app/lib/backend/resources/arduino-cli.exe")
        print(arduino.version())
        # Ottieni informazioni sulla scheda ESP32-S3
        boards_info = arduino.board.list()
        print(boards_info)

        return render_template('welcome.html')


@app.route('/install_page')
def install_page():
    return render_template('install.html')


@app.route('/install/firmware', methods=['GET', 'POST'])
def install_page_firmware():
    if request.method == 'POST':
        selected_port = request.form['selected_port']
        print(selected_port)
        if selected_port:
            save_port_to_json(selected_port)
            upload_result = upload_firmware(selected_port, firmware_path_file)  # Carica il firmware sull'ESP32-S3
            print(upload_result)
            if 'error' in upload_result:
                error_message = upload_result['error']
                return render_template('install.html', error_message=error_message)
            return redirect(url_for('index'))  # Redirect to index page
    return render_template('install.html')


if __name__ == '__main__':
    app.run(debug=True)
