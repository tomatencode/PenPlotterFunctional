#include "StepperAxis.hpp"
#include "Stepper.hpp"
#include "MotorDriver.hpp"

StepperAxis::StepperAxis(Stepper& stepper, MotorDriver& driver)
    : _stepper(stepper), _positionSteps(0), _driver(driver) {}

void StepperAxis::step(bool clockwise) {
    _stepper.setDirection(clockwise);
    _stepper.step();
    _positionSteps += (clockwise ? 1 : -1) / _driver.getMicrosteps();;
}

void StepperAxis::zero() { _positionSteps = 0; }
float StepperAxis::positionSteps() const { return _positionSteps; }