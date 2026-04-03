#pragma once

#include "CoreXYKinematics.hpp"
#include "StepperAxis.hpp"
#include "systemServices/MotionState.hpp"

class BezierExecuter {
public:
    BezierExecuter(StepperAxis& axisA, StepperAxis& axisB, CoreXYKinematics& kinematics, MotionState& motionState);

    void bezierTo(const XYPos& targetPos, double mm_per_s);
    XYPos getCurrentPos() const;

private:
    StepperAxis& _axisA;
    StepperAxis& _axisB;
    CoreXYKinematics& _kinematics;
    MotionState& _motionState;
};
