// ServoManager.h
#ifndef ServoManager_h
#define ServoManager_h

#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>

// Pin predefiniti per i servo motori
#define HEAD_SERVO_PIN 5
#define PUSH_R_SERVO_PIN 4
#define PUSH_L_SERVO_PIN 3
#define ROTATE_R_SERVO_PIN 2
#define ROTATE_L_SERVO_PIN 1
#define BASE_SERVO_PIN 0

class ServoManager {
public:
  static ServoManager &getInstance(Adafruit_PWMServoDriver *pwm, uint16_t minPulse, uint16_t maxPulse);

  void write(uint16_t angle);
  void moveToAngle(uint16_t angle, uint16_t delay);

private:
  ServoManager(Adafruit_PWMServoDriver *pwm, uint16_t minPulse, uint16_t maxPulse);
  ServoManager(const ServoManager &) = delete;
  ServoManager &operator=(const ServoManager &) = delete;

  Adafruit_PWMServoDriver *pwm;
  uint16_t minPulse;
  uint16_t maxPulse;
  uint16_t servoPin;
};

#endif