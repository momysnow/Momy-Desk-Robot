from flask import Blueprint
import subprocess
import os
import platform

driver_bp = Blueprint('driver', __name__)

# Link per scaricare il driver CH343SER Windows
driver_link_windows = "https://github.com/Freenove/Freenove_ESP32_S3_WROOM_Board/blob/05388d3ab045db4336c915482e185e4c4d77aaa1/CH343/Windows/CH343SER.EXE"


@driver_bp.route('/install/driver')
def install_driver():
    # Eseguire i controlli e installare il driver se necessario
    if not is_ch343ser_driver_installed():
        print("Il driver CH343SER non è installato.")
        if os == 'Windows':
            install_ch343ser_driver_windows(driver_link_windows)
        else:
            print("Installa il driver manualmente.")
    else:
        print("Il driver CH343SER è installato.")


# Funzione per controllare il sistema operativo
def get_os():
    return platform.system()


# Funzione per controllare se il driver CH343SER è installato
def is_ch343ser_driver_installed():
    os_type = get_os()
    if os_type == 'Linux':
        try:
            lsmod_output = subprocess.check_output(["lsmod"])
            return b"ch34x" in lsmod_output
        except subprocess.CalledProcessError:
            return False
    elif os_type == 'Windows':
        try:
            reg_output = subprocess.check_output(
                ["reg", "query", "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\ch341"])
            return b"Error" not in reg_output
        except subprocess.CalledProcessError:
            return False
    elif os_type == 'Darwin':  # macOS
        try:
            kextstat_output = subprocess.check_output(["kextstat"])
            return b"com.wch.usbserial" in kextstat_output
        except subprocess.CalledProcessError:
            return False
    return False


# Funzione per installare il driver CH343SER su Windows
def install_ch343ser_driver_windows(driver_link):
    # Apri il browser predefinito con il link per scaricare il driver
    try:
        subprocess.run(["start", driver_link], shell=True)
        print("Per favore, installa manualmente il driver CH343SER per Windows dal link aperto nel browser.")
    except Exception as e:
        print("Errore durante l'apertura del browser:", e)


# Funzione per installare il driver CH343SER su macOS
def install_ch343ser_driver_macos(driver_link):
    # Apri il browser predefinito con il link per scaricare il driver
    try:
        subprocess.run(["open", driver_link])
        print("Per favore, installa manualmente il driver CH343SER per macOS dal link aperto nel browser.")
    except Exception as e:
        print("Errore durante l'apertura del browser:", e)


# Funzione per installare il driver CH343SER su Linux
def install_ch343ser_driver_linux(driver_link):
    print("Per favore, installa manualmente il driver CH343SER per Linux dal seguente link:")
    print(driver_link)