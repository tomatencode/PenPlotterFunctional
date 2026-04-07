#pragma once
#include <cstdint>

struct MotorSteps {
    double a;
    double b;
};

struct XYPos {
    double xMm;
    double yMm;
};

class CoreXYKinematics {
public:
    explicit CoreXYKinematics(double newStepsPerMm);

    MotorSteps mmToSteps(const XYPos& pos) const;
    XYPos stepsToMm(const MotorSteps& steps) const;

private:
    double _stepsPerMm;
};