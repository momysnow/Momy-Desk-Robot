#ifndef CameraUploader_h
#define CameraUploader_h

#include <Arduino.h>
#include "esp_camera.h"
#include <WiFiManager.h>
#define CAMERA_MODEL_ESP32S3_EYE

class CameraUploader {
public:
  CameraUploader(const char* huggingFaceApiUrl, const char* huggingFaceApiToken);
  void begin();
  void uploadImage();

private:
  const char* huggingFaceApiUrl;
  const char* huggingFaceApiToken;
  WiFiClient client;

  void cameraSetup();
};

#endif
