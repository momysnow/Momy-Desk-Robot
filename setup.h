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

// camera pins
#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 15
#define SIOD_GPIO_NUM 4
#define SIOC_GPIO_NUM 5

#define Y2_GPIO_NUM 11
#define Y3_GPIO_NUM 9
#define Y4_GPIO_NUM 8
#define Y5_GPIO_NUM 10
#define Y6_GPIO_NUM 12
#define Y7_GPIO_NUM 18
#define Y8_GPIO_NUM 17
#define Y9_GPIO_NUM 16

#define VSYNC_GPIO_NUM 6
#define HREF_GPIO_NUM 7
#define PCLK_GPIO_NUM 13

#endif