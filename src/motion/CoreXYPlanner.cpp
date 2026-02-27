#include "CoreXYPlanner.hpp"
#include <cmath>
#include <algorithm>

void MoveToXY(
    const MotorSteps& currentSteps,
    const XYPos& targetPos,
    float stepsPerMM,
    StepCallback stepperACallback,
    StepCallback stepperBCallback
) {
    // Convert target position (mm) → motor steps
    MotorSteps targetSteps = mm_to_steps(targetPos, stepsPerMM);

    int32_t deltaA = targetSteps.a - currentSteps.a;
    int32_t deltaB = targetSteps.b - currentSteps.b;

    int32_t absA = std::abs(deltaA);
    int32_t absB = std::abs(deltaB);

    bool dirA = (deltaA >= 0);
    bool dirB = (deltaB >= 0);

    int32_t stepsInLoop = std::max(absA, absB);
    if (stepsInLoop == 0) return; // already at target

    long errA = 0;
    long errB = 0;

    for (int32_t i = 0; i < stepsInLoop; i++) {
        errA += absA;
        errB += absB;

        if (errA >= stepsInLoop) {
            stepperACallback(dirA);
            errA -= stepsInLoop;
        }
        if (errB >= stepsInLoop) {
            stepperBCallback(dirB);
            errB -= stepsInLoop;
        }
    }
}