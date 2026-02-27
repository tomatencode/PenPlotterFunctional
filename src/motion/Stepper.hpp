#ifndef STEPPER_H
#define STEPPER_H

#include <Arduino.h>

class Stepper {
public:
    // The "Public API" - what your G-code parser will see
    Stepper(uint8_t stepPin, uint8_t dirPin);
    
    void setDirection(bool clockwise);
    void step();
    void pulse(uint32_t count, uint32_t intervalUs);

private:
    // Private data - hidden from the rest of the app
    uint8_t _stepPin;
    uint8_t _dirPin;
};

#endif