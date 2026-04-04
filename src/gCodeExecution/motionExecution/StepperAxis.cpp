#include "StepperAxis.hpp"
#include "hardware/steppers/Stepper.hpp"
#include "hardware/drivers/MotorDriver.hpp"

StepperAxis::StepperAxis(Stepper& stepper, MotorDriver& driver, bool flippedDirection)
    : _stepper(stepper), _positionSteps(0), _driver(driver), flippedDirection(flippedDirection) {}

void StepperAxis::step(bool clockwise) {
    _stepper.setDirection(clockwise != flippedDirection);
    _stepper.step();
    _positionSteps += (clockwise ? 1 : -1) / (double)_driver.getMicrosteps();
}

void StepperAxis::setPositionSteps(double steps) {
    _positionSteps = steps;
}

double StepperAxis::positionSteps() const { return _positionSteps; }

uint16_t StepperAxis::microsteps() const {
    return _driver.getMicrosteps();
}