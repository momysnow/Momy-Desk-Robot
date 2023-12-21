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