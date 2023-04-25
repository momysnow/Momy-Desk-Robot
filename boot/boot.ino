// Library

#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include "network.h"
#include <string.h>

#include <PNGdec.h>

#include "logo.h" // Include image array

#include "SPI.h"      // Include the TFT library - see https://github.com/Bodmer/TFT_eSPI for library information
#include <TFT_eSPI.h> // Hardware-specific library

// End Library

// Variable

PNG png; // PNG decoder instance

#define MAX_IMAGE_WDITH 240 // Sets rendering line buffer lengths, adjust for your images

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

// Position variables must be global (PNGdec does not handle position coordinates)
int16_t xpos = 30;
int16_t ypos = 60;

// Definisco i nomi dei file da verificare
const char *file_list[] = {
    "network.h"};

const int file_count = sizeof(file_list) / sizeof(file_list[0]);

// End Variable

// Verifico la presenza di tutti i file richiesti
bool check_files()
{
  for (int i = 0; i < file_count; i++)
  {
    if (!SPIFFS.exists(file_list[i]))
    {
      Serial.printf("File mancante: %s\n", file_list[i]);
      return false;
    }
  }
  return true;
}

void setup()
{
  Serial.begin(115200); // Inizializzo la comunicazione seriale
  SPIFFS.begin();       // Inizializzo il file system SPIFFS

  initialization(); // Inizializzo il sistema
  start_ap();       // Configuro il punto di accesso WiFi

  if (!check_files())
  {
    // Non tutti i file richiesti sono presenti, non posso procedere
    return;
  }

  // Inizializza schermo
  // Mostra logo schermo
  // Pulisci schermo

  // Initialise the TFT
  tft.begin();
  tft.fillScreen(TFT_BLACK);

  Serial.println("\r\nInitialisation done.");
  sleep(200);
  uint16_t pngw = 0, pngh = 0; // To store width and height of image

  int16_t rc = png.openFLASH((uint8_t *)logo_momysnow, sizeof(logo_momysnow), pngDraw);

  if (rc == PNG_SUCCESS)
  {
    Serial.println("Successfully png file");
    pngw = png.getWidth();
    pngh = png.getHeight();
    Serial.printf("Image metrics: (%d x %d), %d bpp, pixel type: %d\n", pngw, pngh, png.getBpp(), png.getPixelType());

    tft.startWrite();
    uint32_t dt = millis();
    rc = png.decode(NULL, 0);
    tft.endWrite();
    Serial.print(millis() - dt);
    Serial.println("ms");
    tft.endWrite();
    sleep(500);
    png.close(); // Required for files, not needed for FLASH arrays
  }

  // Il sistema è stato avviato con successo
  Serial.println("Sistema avviato.");
  // Inizio il server
  WiFiServer server(80);
  server.begin();
}

void loop()
{
  WiFiClient client = server.available();

  if (client)
  {
    // Leggo il comando API inviato dal client
    String API = Network::get_API_command(client);

    // Se il comando ricevuto è "get_networks"
    if (API == "get_networks")
    {
      // Invio la lista delle reti WiFi disponibili al client
      String networks = Network::get_networks();
      client.print(networks);

      // Se il comando ricevuto è "connect_to_network"
    }
    else if (API == "connect_to_network")
    {
      // Leggo il nome della rete WiFi e la password inviati dal client
      String ssid = Network::get_API_command(client);
      String password = Network::get_API_command(client);

      // Provo a connettermi alla rete WiFi
      if (Network::connect_to_network(ssid, password))
      {
        // Se la connessione è andata a buon fine, invio conferma al client
        client.print("WiFi setup completed");
        client.print(Network::get_local_ip());
      }
      else
      {
        // Altrimenti, invio errore al client
        client.print("WiFi setup error");
      }

      // Chiudo la connessione alla rete WiFi
      Network::disconnect_network();

      // Se il comando ricevuto è "reset_device"
    }
    else if (API == "reset_device")
    {
      // Resetto il dispositivo
      ESP.restart();

      // Altrimenti, il comando ricevuto non è valido
    }
    else
    {
      // Invio errore al client
      client.print("Invalid command");
    }

    // Chiudo la connessione con il client
    client.stop();
  }
  delay(10); // Aggiungi un ritardo per evitare l'uso eccessivo della CPU
}