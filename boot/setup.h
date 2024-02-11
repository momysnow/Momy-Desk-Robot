// setup.h
#ifndef SETUP_H
#define SETUP_H

// Microphone
#define I2S_WS 15
#define I2S_SD 13
#define I2S_SCK 2

// touchPad
#define touchPadHeadPin 40

// Servo Motors
#define headServoPin 5
#define pushRServoPin 4
#define pushLServoPin 3
#define rotateRServoPin 2
#define rotateLServoPin 1
#define baseServoPin 0

// API
//weather
const char apiKeyWeather[] = "7ddc20023cdd9038830ba467ab3fcd99";
const char cityName[] = "City";  // Replace with the name of your city

#endif