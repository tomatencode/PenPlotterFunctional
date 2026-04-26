#include <Arduino.h>
#include "Stepper.hpp"

Stepper::Stepper(uint8_t stepPin, uint8_t dirPin) 
    : _stepPin(stepPin), _dirPin(dirPin) {
    pinMode(_stepPin, OUTPUT);
    pinMode(_dirPin, OUTPUT);
}

void Stepper::setDirection(bool clockwise) {
    digitalWrite(_dirPin, clockwise ? HIGH : LOW);
}

void Stepper::step() {
    digitalWrite(_stepPin, HIGH);
    delayMicroseconds(2);
    digitalWrite(_stepPin, LOW);
}