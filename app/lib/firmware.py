import pyduinocli

# C:/Program Files/Arduino IDE/resources/app/lib/backend/resources/arduino-cli.exe

firmware_path_file = "C:/Users/stefa/Desktop/Momy2/firmware_test/test_upload_firmware/test_upload_firmware.ino"  # Percorso del firmware da caricare


def upload_firmware(port, firmware_path):
    arduino = pyduinocli.Arduino("C:/Program Files/Arduino IDE/resources/app/lib/backend/resources/arduino-cli.exe")

    try:
        arduino.compile(fqbn="esp32:esp32:esp32s3", sketch=firmware_path)
        upload_result = arduino.upload(fqbn="esp32:esp32:esp32s3", sketch=firmware_path, port=port)
        if 'error' in upload_result:
            print("Errore durante il caricamento del firmware:", upload_result['error'])
            return {'success': False, 'error': upload_result['error']}
        print("Firmware caricato con successo!")
        return {'success': True}
    except Exception as e:
        print("Errore durante il caricamento del firmware:", str(e))
        return {'success': False, 'error': str(e)}
