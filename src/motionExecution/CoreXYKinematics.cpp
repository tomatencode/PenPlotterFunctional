#include "CoreXYKinematics.hpp"

CoreXYKinematics::CoreXYKinematics(double steps_per_mm_) : _stepsPerMm(steps_per_mm_) {}

// Convert X/Y space in mm to motor step space
MotorSteps CoreXYKinematics::mmToSteps(const XYPos& pos) const {
    MotorSteps m;
    m.a = (pos.xMm + pos.yMm) * _stepsPerMm;
    m.b = (pos.xMm - pos.yMm) * _stepsPerMm;
    return m;
}

// Convert motor step space to X/Y space in mm
XYPos CoreXYKinematics::stepsToMm(const MotorSteps& steps) const {
    XYPos pos;
    pos.xMm = (steps.a + steps.b) / (2.0 * _stepsPerMm);
    pos.yMm = (steps.a - steps.b) / (2.0 * _stepsPerMm);
    return pos;
}