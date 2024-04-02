// Microphone_h.h

#ifndef Microphone_h
#define Microphone_h

#include <driver/i2s.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "setup.h"

#define I2S_PORT I2S_NUM_0
#define I2S_SAMPLE_RATE   (16000)
#define I2S_SAMPLE_BITS   (16)
#define I2S_READ_LEN      (16 * 1024)
#define RECORD_TIME       (1) //Seconds
#define I2S_CHANNEL_NUM   (1)
#define FLASH_RECORD_SIZE (I2S_CHANNEL_NUM * I2S_SAMPLE_RATE * I2S_SAMPLE_BITS / 8 * RECORD_TIME)

const char filename[] = "/recording.wav";
const int headerSize = 44;
extern File file;  // Declare file as extern
extern bool isWIFIConnected;  // Declare isWIFIConnected as extern


void SPIFFSInit();
void i2sInit();
void i2s_adc_data_scale(uint8_t * d_buff, uint8_t* s_buff, uint32_t len);
void i2s_adc(void *arg);
void example_disp_buf(uint8_t* buf, int length);
void wavHeader(byte* header, int wavSize);
void listSPIFFS(void);
void uploadFile();

#endif


/* POSSIBILE AGGIUNTA

Per introdurre la possibilità di rilevare una parola di attivazione prima di ascoltare il comando, puoi utilizzare una libreria come Porcupine di Picovoice. Porcupine è una libreria per il riconoscimento delle parole di attivazione (wake word detection) che può essere integrata nel tuo progetto.

Ecco come potresti modificare il tuo codice per includere il rilevamento della parola di attivazione:

1. Installa la libreria Porcupine seguendo le istruzioni fornite nella documentazione ufficiale: https://github.com/Picovoice/porcupine

2. Modifica il file `Microphone_h.h` per includere l'header della libreria Porcupine:

```cpp
#include <pv_porcupine.h>
```

3. Nel file `AudioRecorder.cpp`, aggiungi una funzione per inizializzare Porcupine con la parola di attivazione desiderata:

```cpp
void initPorcupine() {
  pv_porcupine_t *handle;
  const char *access_key = "YOUR_ACCESS_KEY";
  const char *keyword_path = "path/to/your/keyword/file.ppn";
  float sensitivity = 0.5f;
  
  pv_status_t status = pv_porcupine_init(access_key, keyword_path, sensitivity, &handle);
  if (status != PV_STATUS_SUCCESS) {
    Serial.println("Failed to initialize Porcupine");
    while(1);
  }
}
```

Assicurati di sostituire `"YOUR_ACCESS_KEY"` con la tua chiave di accesso Porcupine e `"path/to/your/keyword/file.ppn"` con il percorso del file della parola di attivazione.

4. Modifica la funzione `i2s_adc` per integrare il rilevamento della parola di attivazione:

```cpp
void i2s_adc(void *arg) {
  // ...
  
  bool wakeWordDetected = false;
  
  while (flash_wr_size < FLASH_RECORD_SIZE) {
    i2s_read(I2S_PORT, (void*)i2s_read_buff, i2s_read_len, &bytes_read, portMAX_DELAY);
    
    if (!wakeWordDetected) {
      int result = pv_porcupine_process(handle, (int16_t *)i2s_read_buff, &wakeWordDetected);
      if (result != PV_STATUS_SUCCESS) {
        Serial.println("Error processing audio frame");
      }
      
      if (wakeWordDetected) {
        Serial.println("Wake word detected!");
      }
    } else {
      i2s_adc_data_scale(flash_write_buff, (uint8_t*)i2s_read_buff, i2s_read_len);
      file.write((const byte*)flash_write_buff, i2s_read_len);
      flash_wr_size += i2s_read_len;
      // ...
    }
  }
  
  // ...
}
```

In questo modo, il codice inizierà a registrare l'audio solo dopo aver rilevato la parola di attivazione utilizzando Porcupine.

5. Nel file `AudioRecorder.cpp`, chiama la funzione `initPorcupine()` prima di avviare la registrazione audio.

Ricorda di liberare le risorse di Porcupine quando hai finito di utilizzarlo, chiamando `pv_porcupine_delete(handle)`.

Questi sono i passaggi principali per integrare il rilevamento della parola di attivazione utilizzando Porcupine nella tua libreria. Assicurati di seguire attentamente la documentazione di Porcupine per ulteriori dettagli sull'utilizzo e la configurazione della libreria.
*/