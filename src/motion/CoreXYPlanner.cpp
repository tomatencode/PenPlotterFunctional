#include "CoreXYPlanner.hpp"
#include <cmath>
#include <algorithm>
#include <Arduino.h>

void MoveToXY(
    const MotorSteps& currentSteps,
    const XYPos& currentPos,
    const XYPos& targetPos,
    float stepsPerMM,
    float mm_per_s,
    StepCallback stepperACallback,
    StepCallback stepperBCallback
) {
    float dx = targetPos.x_mm - currentPos.x_mm;
    float dy = targetPos.y_mm - currentPos.y_mm;
    float distance_mm = std::sqrt(dx * dx + dy * dy);
    if (distance_mm <= 0.0f) return;

    MotorSteps targetSteps = mm_to_steps(targetPos, stepsPerMM);

    int32_t deltaA = targetSteps.a - currentSteps.a;
    int32_t deltaB = targetSteps.b - currentSteps.b;

    int32_t absA = std::abs(deltaA);
    int32_t absB = std::abs(deltaB);

    bool dirA = (deltaA >= 0);
    bool dirB = (deltaB >= 0);

    int32_t stepsInLoop = std::max(absA, absB);
    if (stepsInLoop == 0) return;

    // ---- timing calculation ----
    float move_time_s = distance_mm / mm_per_s;
    float step_frequency = stepsInLoop / move_time_s;
    uint32_t step_interval_us = static_cast<uint32_t>(1e6f / step_frequency);

    uint32_t next_step_time = micros();

    long errA = 0;
    long errB = 0;

    for (int32_t i = 0; i < stepsInLoop; i++) {

        // Wait until scheduled time
        while ((int32_t)(micros() - next_step_time) < 0) {
            yield();
        }

        next_step_time += step_interval_us;

        // ---- stepping logic ----
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