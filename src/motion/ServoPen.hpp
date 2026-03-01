#ifndef SERVO_PEN_HPP
#define SERVO_PEN_HPP

#include <ESP32Servo.h>
#include "Pen.hpp"

class ServoPen : public Pen {
public:
    ServoPen(Servo& servo, int upPosition, int downPosition);

    void down() override;
    void up() override;

private:
    Servo& _servo;
    int _upPosition;
    int _downPosition;
};

#endif