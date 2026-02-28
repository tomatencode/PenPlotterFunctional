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

class CoreXYKinematics {
public:
    explicit CoreXYKinematics(float steps_per_mm_) : steps_per_mm(steps_per_mm_) {}

    MotorSteps mm_to_steps(const XYPos& pos) const {
        return { pos.x_mm * steps_per_mm, pos.y_mm * steps_per_mm };
    }

    XYPos steps_to_mm(const MotorSteps& steps) const {
        return { steps.a / steps_per_mm, steps.b / steps_per_mm };
    }

private:
    float steps_per_mm;
};
#endif