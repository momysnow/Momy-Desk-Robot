#include "setup.h"


#include <TFT_eSPI.h>
#include "Logo.h"

#include "finger_press_line.h"
#include "save_fill.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite eyes = TFT_eSprite(&tft);

const unsigned int sreenW = 240;
const unsigned int sreenH = 240;

#include <ESP32Servo.h>

#include <Arduino.h>
#include <EEPROM.h>
int addr = 0;
#define EEPROM_SIZE 4

// servo
Servo controlServo;

int pos = 0;
int lastPosition = -1;  // Initialized to an impossible value

void setup() {
  Serial.begin(115200);

  if (!EEPROM.begin(EEPROM_SIZE)) {
    Serial.println("failed to initialise EEPROM");
  }

  // setup Screen
  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);
  // Show Logo
  tft.pushImage(30, 60, logoWidth, logoHeight, logo_momysnow);

  // setup servo
  ESP32PWM::allocateTimer(0);
  controlServo.setPeriodHertz(50);
  // HEAD
  controlServo.attach(headServoPin, 1000, 2000);
  controlServo.write(90);
  controlServo.detach();
  // PUSH_L
  controlServo.attach(pushLServoPin, 1000, 2000);
  controlServo.write(90);
  controlServo.detach();
  // PUSH_R
  controlServo.attach(pushRServoPin, 1000, 2000);
  controlServo.write(90);
  controlServo.detach();
  // ROTATE_L
  controlServo.attach(rotateLServoPin, 1000, 2000);
  controlServo.write(90);
  controlServo.detach();
  // ROTATE_R
  controlServo.attach(rotateRServoPin, 1000, 2000);
  controlServo.write(90);
  controlServo.detach();
  // BASE
  controlServo.attach(baseServoPin, 1000, 2000);
  controlServo.write(90);
  controlServo.detach();

  delay(2000);

  tft.fillScreen(TFT_BLACK);
  delay(500);

  // SHOW IMG NOT TOUCH AND SAVE VAlUE
  tft.pushImage((240 - wifiWidth) / 2, (240 - wifiHeight) / 2, wifiWidth, wifiHeight, finger_press_fill_stop);

  // save touchRead(touchPadHeadPin) without touching
  // initialize a variable to store the sum of the sensor values
  int sum = 0;
  // initialize a counter to keep track of the number of readings
  int count = 0;
  // set the maximum number of readings to 200 (corresponding to 2 seconds with a delay of 10 ms)
  int maxCount = 100;
  // repeat the loop until the counter reaches the maximum
  while (count < maxCount) {
    // add the current sensor value to the sum
    sum += touchRead(touchPadHeadPin);
    // increment the counter
    count++;
    // wait for 10 ms
    delay(100);
  }

  // calculate the average value by dividing the sum by the count
  int averageValue = sum / count;
  
  EEPROM.put(addr, averageValue);                 // write the value to the EEPROM
  Serial.print("noTouchValue: ");
  Serial.println(averageValue);

  delay(3000);
  tft.fillScreen(TFT_BLACK);
  delay(100);

  // SHOW IMG TOUCH AND SAVE VAlUE
  tft.pushImage((240 - wifiWidth) / 2, (240 - wifiHeight) / 2, wifiWidth, wifiHeight, finger_press_fill);

  // wait until a touch is detected based on the difference between touch and no touch values
  int threshold = 5000;                                   // adjust this value as needed
  int currentValue = touchRead(touchPadHeadPin);          // read the current sensor value
  while (abs(currentValue - averageValue) < threshold) {  // check if the difference is less than the threshold
    currentValue = touchRead(touchPadHeadPin);            // update the current sensor value
    delay(10);                                            // wait for a short time
  }

  // initialize a variable to store the sum of the sensor values
  sum = 0;
  // initialize a counter to keep track of the number of readings
  count = 0;
  // set the maximum number of readings to 200 (corresponding to 2 seconds with a delay of 10 ms)
  maxCount = 100;
  // repeat the loop until the counter reaches the maximum
  while (count < maxCount) {
    // add the current sensor value to the sum
    sum += touchRead(touchPadHeadPin);
    // increment the counter
    count++;
    // wait for 10 ms
    delay(100);
  }

  // calculate the average value by dividing the sum by the count
  averageValue = sum / count;
  // write the average value to the next EEPROM address
  EEPROM.put(addr + 2, averageValue);
  // print the average value
  Serial.println("");
  Serial.print("averageValue: ");
  Serial.println(averageValue);

  delay(3000);
  tft.fillScreen(TFT_BLACK);

  // SHOW IMG SAVE
  tft.pushImage((240 - saveWidth) / 2, (240 - saveHeight) / 2, saveWidth, saveHeight, save_fill);
}

void loop() {
}