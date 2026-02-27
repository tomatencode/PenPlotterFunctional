#include "StepperAxis.hpp"
#include "Stepper.hpp"

StepperAxis::StepperAxis(Stepper& stepper, float (*microstepGetter)())
    : _stepper(stepper), _positionSteps(0), _getMicrosteps(microstepGetter) {}

void StepperAxis::step(bool clockwise) {
    _stepper.setDirection(clockwise);
    _stepper.step();
    _positionSteps += clockwise ? 1 : -1 / _getMicrosteps();
}

void StepperAxis::zero() { _positionSteps = 0; }
float StepperAxis::positionSteps() const { return _positionSteps; }