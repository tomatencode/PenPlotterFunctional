#include "StepperAxis.hpp"
#include "Stepper.hpp"
#include "MotorDriver.hpp"

StepperAxis::StepperAxis(Stepper& stepper, MotorDriver& driver)
    : _stepper(stepper), _positionSteps(0), _driver(driver) {}

void StepperAxis::step(bool clockwise) {
    _stepper.setDirection(clockwise);
    _stepper.step();
    _positionSteps += (clockwise ? 1 : -1) / (float)_driver.getMicrosteps();
}

void StepperAxis::setPositionSteps(float steps) {
    _positionSteps = steps;
}

float StepperAxis::positionSteps() const { return _positionSteps; }

uint16_t StepperAxis::microsteps() const {
    return _driver.getMicrosteps();
}