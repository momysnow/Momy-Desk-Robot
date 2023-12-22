// setup.h
#ifndef SETUP_H
#define SETUP_H

// Microphone
#define I2S_WS 15
#define I2S_SD 13
#define I2S_SCK 2

// DHT11
#define dhtPin 11

// touchPad
#define touchPadHeadPin 3

// Servo Motors
#define headServoPin 9
#define pushLServoPin 10
#define pushRServoPin 11
#define rotateLServoPin 12
#define rotateRServoPin 13
#define baseServoPin 14  // control the pin is unused

// API
//weather
const char apiKeyWeather[] = "7ddc20023cdd9038830ba467ab3fcd99";
const char cityName[] = "City";  // Replace with the name of your city

#endif