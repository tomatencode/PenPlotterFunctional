#include "CoreXYKinematics.hpp"

CoreXYKinematics::CoreXYKinematics(double steps_per_mm_) : steps_per_mm(steps_per_mm_) {}

// Convert X/Y in mm to motor steps
MotorSteps CoreXYKinematics::mm_to_steps(const XYPos& pos) const {
    MotorSteps m;
    m.a = (pos.x_mm + pos.y_mm) * steps_per_mm;
    m.b = (pos.x_mm - pos.y_mm) * steps_per_mm;
    return m;
}

// Convert motor steps to X/Y in mm
XYPos CoreXYKinematics::steps_to_mm(const MotorSteps& steps) const {
    XYPos pos;
    pos.x_mm = (steps.a + steps.b) / (2.0 * steps_per_mm);
    pos.y_mm = (steps.a - steps.b) / (2.0 * steps_per_mm);
    return pos;
}