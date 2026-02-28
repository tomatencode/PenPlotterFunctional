#ifndef COREXY_PLANNER_H
#define COREXY_PLANNER_H

#include <cstdint>
#include "CoreXYKinematics.hpp"

using StepCallback = void(*)(bool direction);

// Plan and execute a linear move directly using a callback
void MoveToXY(
    const MotorSteps& currentSteps,
    const XYPos& targetPos,
    float mm_per_s,
    const CoreXYKinematics& kinematics,
    StepCallback stepperACallback,
    StepCallback stepperBCallback
);

#endif