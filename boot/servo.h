// Servo.h
#ifndef Servo_h
#define Servo_h

#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>

// Pin predefiniti per i servo motori
#define HEAD_SERVO_PIN 5
#define PUSH_R_SERVO_PIN 4
#define PUSH_L_SERVO_PIN 3
#define ROTATE_R_SERVO_PIN 2
#define ROTATE_L_SERVO_PIN 1
#define BASE_SERVO_PIN 0

class Servo {
public:
    static Servo& getInstance();
    Servo(Adafruit_PWMServoDriver *pwm, uint16_t minPulse, uint16_t maxPulse);

    Servo& getHeadServo();
    Servo& getPushRServo();
    Servo& getPushLServo();
    Servo& getRotateRServo();
    Servo& getRotateLServo();
    Servo& getBaseServo();

    void write(uint16_t angle);
    void moveToAngle(uint16_t angle, uint16_t delay);

private:
    Adafruit_PWMServoDriver *pwm;
    uint16_t minPulse;
    uint16_t maxPulse;

    Servo *headServo;
    Servo *pushRServo;
    Servo *pushLServo;
    Servo *rotateRServo;
    Servo *rotateLServo;
    Servo *baseServo;
};

#endif

/* Example code
#include "Servo.h"

void setup() {
    Servo& servo = Servo::getInstance();

    // Configurazione iniziale
    servo.pwm->begin();
    servo.pwm->setOscillatorFrequency(27000000);
    servo.pwm->setPWMFreq(SERVO_FREQ);
}

void loop() {
    Servo& servo = Servo::getInstance();

    // Controlla il servo motore della testa
    servo.getHeadServo().write(90);
    delay(1000);
    servo.getHeadServo().moveToAngle(180, 10);
    delay(1000);
    servo.getHeadServo().moveToAngle(0, 10);
    delay(1000);

    // Controlla gli altri servo motori
    servo.getPushRServo().write(45);
    // ...
}
*/