#include <Arduino.h>
#include "ServoManager.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

ServoManager& servo = ServoManager::getInstance(&pwm, 100, 500);

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(50);
}

void loop() {
  // Test write() method
  Serial.println("Testing write() method");
  servo.write(0);
  delay(1000);
  servo.write(90);
  delay(1000);
  servo.write(180);
  delay(1000);

  // Test moveToAngle() method
  Serial.println("Testing moveToAngle() method");
  servo.moveToAngle(0, 10);
  delay(1000);
  servo.moveToAngle(90, 10);
  delay(1000);
  servo.moveToAngle(180, 10);
  delay(1000);

  // Test moveToAngle() method with different delay
  Serial.println("Testing moveToAngle() method with different delay");
  servo.moveToAngle(0, 50);
  delay(1000);
  servo.moveToAngle(90, 20);
  delay(1000);
  servo.moveToAngle(180, 10);
  delay(1000);
}