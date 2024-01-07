// setup.h
#ifndef SETUP_H
#define SETUP_H

// Microphone
#define I2S_WS 15
#define I2S_SD 13
#define I2S_SCK 2

// DHT11
#define dhtPin 39

// touchPad
#define touchPadHeadPin 40

// Servo Motors
#define headServoPin 47
#define pushLServoPin 48
#define pushRServoPin 45
#define rotateLServoPin 0
#define rotateRServoPin 35
#define baseServoPin 38

// API
//weather
const char apiKeyWeather[] = "7ddc20023cdd9038830ba467ab3fcd99";
const char cityName[] = "City";  // Replace with the name of your city

#endif