#include "StepperAxis.hpp"
#include "Stepper.hpp"

StepperAxis::StepperAxis(Stepper& stepper, float microsteps)
    : _stepper(stepper), _positionSteps(0), _microsteps(microsteps) {}

void StepperAxis::step(bool clockwise) {
    _stepper.setDirection(clockwise);
    _stepper.step();
    _positionSteps += clockwise ? 1 : -1 / _microsteps;
}

void StepperAxis::zero() { _positionSteps = 0; }
float StepperAxis::positionSteps() const { return _positionSteps; }