#ifndef COREXY_KINEMATICS_H
#define COREXY_KINEMATICS_H

#include <cstdint>

struct MotorSteps {
    double a;
    double b;
};

struct XYPos {
    double x_mm;
    double y_mm;
};

class CoreXYKinematics {
public:
    explicit CoreXYKinematics(double steps_per_mm_);

    MotorSteps mm_to_steps(const XYPos& pos) const;
    XYPos steps_to_mm(const MotorSteps& steps) const;

private:
    double steps_per_mm;
};
#endif