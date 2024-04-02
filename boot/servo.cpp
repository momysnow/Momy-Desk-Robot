// Servo.cpp
#include "Servo.h"

Servo& Servo::getInstance() {
    static Servo instance(nullptr, SERVOMIN, SERVOMAX);
    return instance;
}

Servo::Servo(Adafruit_PWMServoDriver *pwm, uint16_t minPulse, uint16_t maxPulse) {
    this->pwm = pwm;
    this->minPulse = minPulse;
    this->maxPulse = maxPulse;

    headServo = new Servo(pwm, HEAD_SERVO_PIN, minPulse, maxPulse);
    pushRServo = new Servo(pwm, PUSH_R_SERVO_PIN, minPulse, maxPulse);
    pushLServo = new Servo(pwm, PUSH_L_SERVO_PIN, minPulse, maxPulse);
    rotateRServo = new Servo(pwm, ROTATE_R_SERVO_PIN, minPulse, maxPulse);
    rotateLServo = new Servo(pwm, ROTATE_L_SERVO_PIN, minPulse, maxPulse);
    baseServo = new Servo(pwm, BASE_SERVO_PIN, minPulse, maxPulse);
}

Servo& Servo::getHeadServo() {
    return *headServo;
}

Servo& Servo::getPushRServo() {
    return *pushRServo;
}

Servo& Servo::getPushLServo() {
    return *pushLServo;
}

Servo& Servo::getRotateRServo() {
    return *rotateRServo;
}

Servo& Servo::getRotateLServo() {
    return *rotateLServo;
}

Servo& Servo::getBaseServo() {
    return *baseServo;
}

void Servo::write(uint16_t angle) {
    uint16_t pulseLength = map(angle, 0, 180, minPulse, maxPulse);
    pwm->setPWM(servoPin, 0, pulseLength);
}

void Servo::moveToAngle(uint16_t angle, uint16_t delay) {
    uint16_t currentAngle = map(pwm->getPWM(servoPin), minPulse, maxPulse, 0, 180);
    int direction = angle > currentAngle ? 1 : -1;

    for (uint16_t i = currentAngle; i != angle; i += direction) {
        write(i);
        delay(delay);
    }
    write(angle);
}