#include <Arduino.h>
#include "CameraUploader.h"

const char* huggingFaceApiUrl = "https://api-inference.huggingface.co/models/facebook/detr-resnet-50";
const char* huggingFaceApiToken = "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"; // Replace with your Hugging Face API token
// const char* huggingFaceApiToken = "<your_hugging_face_api_token>"; // Replace with your Hugging Face API token

CameraUploader uploader(huggingFaceApiUrl, huggingFaceApiToken);

void setup() {
  uploader.begin();
}

void loop() {
  uploader.uploadImage();
  delay(30000); // Upload an image every 30 seconds
}