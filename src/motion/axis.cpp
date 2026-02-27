#include "axis.hpp"
#include "stepper.hpp"

Axis::Axis(Stepper& stepper)
    : _stepper(stepper) {}

void Axis::step(int direction) {
    _stepper.setDirection(direction > 0);
    _stepper.step();
    _positionSteps += direction;
}

void Axis::zero() { _positionSteps = 0; }
int32_t Axis::positionSteps() const { return _positionSteps; }