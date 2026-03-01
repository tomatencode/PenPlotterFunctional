#include "MotionSystem.hpp"
#include <cmath>
#include <algorithm>
#include <Arduino.h>


MotionSystem::MotionSystem(StepperAxis& axisA, StepperAxis& axisB, CoreXYKinematics& kinematics)
    : _axisA(axisA), _axisB(axisB), _kinematics(kinematics) {}

void MotionSystem::moveToXY(
    const XYPos& targetPos,
    float mm_per_s
) {
    // derive current position from steps for motion planning
    MotorSteps currentSteps = {_axisA.positionSteps(), _axisB.positionSteps()};
    uint16_t msA = _axisA.microsteps();
    uint16_t msB = _axisB.microsteps();

    XYPos currentPos = _kinematics.steps_to_mm(currentSteps);

    float dx = targetPos.x_mm - currentPos.x_mm;
    float dy = targetPos.y_mm - currentPos.y_mm;
    
    float distance_mm = std::sqrt(dx * dx + dy * dy);
    if (distance_mm <= 0.0f) return;

    MotorSteps dfullsteps = _kinematics.mm_to_steps({dx, dy});

    // convert deltas from full steps to microsteps
    int32_t deltaA = static_cast<int32_t>((dfullsteps.a) * msA);
    int32_t deltaB = static_cast<int32_t>((dfullsteps.b) * msB);

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
            yield(); // for watchdog (idk if this is necessary)
        }

        next_step_time += step_interval_us;

        // ---- stepping logic ----
        errA += absA;
        errB += absB;

        if (errA >= stepsInLoop) {
            _axisA.step(dirA);
            errA -= stepsInLoop;
        }

        if (errB >= stepsInLoop) {
            _axisB.step(dirB);
            errB -= stepsInLoop;
        }
    }
}