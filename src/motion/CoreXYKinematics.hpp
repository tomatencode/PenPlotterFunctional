#ifndef COREXY_KINEMATICS_H
#define COREXY_KINEMATICS_H

#include <cstdint>

struct MotorSteps {
    float a;
    float b;
};

struct XYPos {
    float x_mm;
    float y_mm;
};

// Convert real-world X/Y (mm) → CoreXY motor steps
MotorSteps mm_to_steps(const XYPos& pos, float steps_per_mm);

// Convert motor steps → real-world X/Y (mm)
XYPos steps_to_mm(const MotorSteps& steps, float steps_per_mm);

#endif