#include "MotionSystem.hpp"
#include <cmath>
#include <algorithm>
#include <Arduino.h>


MotionSystem::MotionSystem(StepperAxis& axisA, StepperAxis& axisB, CoreXYKinematics& kinematics, double min_feature_size_mm)
    : _axisA(axisA), _axisB(axisB), _kinematics(kinematics), _min_feature_size_mm(min_feature_size_mm) {}

void MotionSystem::moveToXY(
    const XYPos& targetPos,
    double mm_per_s
) {
    // derive current position from steps for motion planning
    MotorSteps currentSteps = {_axisA.positionSteps(), _axisB.positionSteps()};
    uint16_t msA = _axisA.microsteps();
    uint16_t msB = _axisB.microsteps();

    XYPos currentPos = _kinematics.steps_to_mm(currentSteps);

    float dx = targetPos.x_mm - currentPos.x_mm;
    float dy = targetPos.y_mm - currentPos.y_mm;
    
    double distance_mm = std::sqrt(dx * dx + dy * dy);
    if (distance_mm <= 0.0) return;

    if (mm_per_s <= 0.0) return;

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
    double move_time_s = distance_mm / mm_per_s;
    double step_frequency = stepsInLoop / move_time_s;
    uint32_t step_interval_us = static_cast<uint32_t>(1e6 / step_frequency);
    if (step_interval_us == 0) step_interval_us = 1;

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

void MotionSystem::arcToXY(
    const XYPos& targetPos,
    const XYPos& centerPos,
    bool clockwise,
    double mm_per_s
) {
    // derive current position from steps for motion planning
    MotorSteps currentSteps = {_axisA.positionSteps(), _axisB.positionSteps()};
    uint16_t msA = _axisA.microsteps();
    uint16_t msB = _axisB.microsteps();

    if (msA != msB) {
        // For simplicity, require both axes to have the same microstepping
        // In a real implementation, we could handle this more gracefully
        Serial.println("Error: microstepping mismatch between axes");
        return;
    }
    double ms = msA; // or msB, since they are equal

    XYPos currentPos = _kinematics.steps_to_mm(currentSteps);

    // Calculate the radius and angles
    double dx = currentPos.x_mm - centerPos.x_mm;
    double dy = currentPos.y_mm - centerPos.y_mm;
    double radius = std::sqrt(dx * dx + dy * dy);

    double startAngle = atan2(currentPos.y_mm - centerPos.y_mm, currentPos.x_mm - centerPos.x_mm);
    double endAngle = atan2(targetPos.y_mm - centerPos.y_mm, targetPos.x_mm - centerPos.x_mm);

    // Calculate the total angle to move through
    double totalAngle;
    if (clockwise) {
        totalAngle = startAngle - endAngle;
        if (totalAngle < 0) totalAngle += 2 * M_PI; // Ensure positive angle
    } else {
        totalAngle = endAngle - startAngle;
        if (totalAngle < 0) totalAngle += 2 * M_PI; // Ensure positive angle
    }

    // Calculate the arc length and time to move
    double arcLength = radius * totalAngle;

    if (_min_feature_size_mm <= 0.0) return;

    int linesegments = static_cast<int>(arcLength / _min_feature_size_mm); // Number of line segments to approximate the arc
    linesegments = std::max(1, std::min(linesegments, 1000));

    for (int32_t i = 1; i <= linesegments; i++) {
        // Calculate the current angle along the arc
        double t = static_cast<double>(i) / (linesegments); // Normalized progress along the arc
        double angle = startAngle + (clockwise ? -1 : 1) * t * totalAngle;
        double x = centerPos.x_mm + radius * cos(angle);
        double y = centerPos.y_mm + radius * sin(angle);
        moveToXY({x, y}, mm_per_s); // Move to the calculated position
    }
}