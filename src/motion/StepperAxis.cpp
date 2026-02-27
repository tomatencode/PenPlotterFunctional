#include "StepperAxis.hpp"
#include "Stepper.hpp"

StepperAxis::StepperAxis(Stepper& stepper)
    : _stepper(stepper) {}

void StepperAxis::step(bool clockwise) {
    _stepper.setDirection(clockwise);
    _stepper.step();
    _positionSteps += clockwise ? 1 : -1;
}

void StepperAxis::zero() { _positionSteps = 0; }
float StepperAxis::positionSteps() const { return _positionSteps; }