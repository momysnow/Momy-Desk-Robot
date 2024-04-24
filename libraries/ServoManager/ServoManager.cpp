// ServoManager.cpp
#include "ServoManager.h"

ServoManager &ServoManager::getInstance(Adafruit_PWMServoDriver *pwm, uint16_t minPulse, uint16_t maxPulse) {
  static ServoManager instance(pwm, minPulse, maxPulse);
  return instance;
}

ServoManager::ServoManager(Adafruit_PWMServoDriver *pwm, uint16_t minPulse, uint16_t maxPulse)
  : pwm(pwm), minPulse(minPulse), maxPulse(maxPulse) {}

void ServoManager::write(uint16_t angle) {
  uint16_t pulseLength = map(angle, 0, 180, minPulse, maxPulse);
  pwm->setPWM(servoPin, 0, pulseLength);
}

void ServoManager::moveToAngle(uint16_t angle, uint16_t delay) {
  uint16_t currentAngle = map(pwm->getPWM(servoPin), minPulse, maxPulse, 0, 180);
  int direction = angle > currentAngle ? 1 : -1;
  for (uint16_t i = currentAngle; i != angle; i += direction) {
    write(i);
    ::delay(delay);
  }
  write(angle);
}