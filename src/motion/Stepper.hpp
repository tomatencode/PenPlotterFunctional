#ifndef STEPPER_H
#define STEPPER_H

#include <Arduino.h>

class Stepper {
public:
    Stepper(uint8_t stepPin, uint8_t dirPin);
    
    void setDirection(bool clockwise);
    void step();
private:
    uint8_t _stepPin;
    uint8_t _dirPin;
};

#endif