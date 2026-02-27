#include "CoreXYKinematics.hpp"

// Convert X/Y in mm to motor steps
MotorSteps mm_to_steps(const XYPos& pos, float steps_per_mm) {
    MotorSteps m;
    m.a = (pos.x_mm + pos.y_mm) * steps_per_mm;
    m.b = (pos.x_mm - pos.y_mm) * steps_per_mm;
    return m;
}

// Convert motor steps to X/Y in mm
XYPos steps_to_mm(const MotorSteps& steps, float steps_per_mm) {
    XYPos pos;
    pos.x_mm = (steps.a + steps.b) / (2.0f * steps_per_mm);
    pos.y_mm = (steps.a - steps.b) / (2.0f * steps_per_mm);
    return pos;
}