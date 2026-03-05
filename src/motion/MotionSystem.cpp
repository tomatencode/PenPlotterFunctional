#include "MotionSystem.hpp"
#include <cmath>
#include <algorithm>
#include <Arduino.h>
#include "../shared/SharedData.hpp"


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

        // Check for motion commands
        if (motionCommand == MotionCommand::PAUSE) {
            telemetry.state = MotionState::MOTION_PAUSED;
            while (motionCommand == MotionCommand::PAUSE)
            {
                yield(); // for watchdog (idk if this is necessary)
            }
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
        if (totalAngle <= 0) totalAngle += 2 * M_PI; // Ensure positive angle
    } else {
        totalAngle = endAngle - startAngle;
        if (totalAngle <= 0) totalAngle += 2 * M_PI; // Ensure positive angle
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

void MotionSystem::quadraticBezierToXY(
    const XYPos& controlPoint,
    const XYPos& targetPos,
    float mm_per_s
) {
    MotorSteps currentSteps = {_axisA.positionSteps(), _axisB.positionSteps()};
    XYPos startPos = _kinematics.steps_to_mm(currentSteps);

    auto bezier = [&](double t) -> XYPos {
        double u = 1.0 - t;
        return {
            u*u*startPos.x_mm + 2*u*t*controlPoint.x_mm + t*t*targetPos.x_mm,
            u*u*startPos.y_mm + 2*u*t*controlPoint.y_mm + t*t*targetPos.y_mm
        };
    };

    const double maxSegmentLength = _min_feature_size_mm;
    double t0 = 0.0;
    XYPos lastPos = startPos;
    const double dt = 0.01; // initial increment

    while (t0 < 1.0) {
        double t1 = t0 + dt;
        if (t1 > 1.0) t1 = 1.0;

        XYPos p1 = bezier(t1);

        // Fully adaptive: increase t1 until segment length >= maxSegmentLength
        while (t1 < 1.0) {
            double dx = p1.x_mm - lastPos.x_mm;
            double dy = p1.y_mm - lastPos.y_mm;
            double dist2 = dx*dx + dy*dy;
            if (dist2 >= maxSegmentLength*maxSegmentLength) break;

            t1 += dt;
            if (t1 > 1.0) t1 = 1.0;
            p1 = bezier(t1);
        }

        moveToXY(p1, mm_per_s);
        lastPos = p1;
        t0 = t1;
    }

    // Only move to exact target if not already there
    double dx = targetPos.x_mm - lastPos.x_mm;
    double dy = targetPos.y_mm - lastPos.y_mm;
    if (dx*dx + dy*dy > 1e-12) moveToXY(targetPos, mm_per_s);
}

void MotionSystem::cubicBezierToXY(
    const XYPos& controlPoint1,
    const XYPos& controlPoint2,
    const XYPos& targetPos,
    float mm_per_s
) {
    // Get current position (start anchor)
    MotorSteps currentSteps = {_axisA.positionSteps(), _axisB.positionSteps()};
    XYPos startPos = _kinematics.steps_to_mm(currentSteps);

    // Lambda to evaluate the cubic Bézier at t
    auto bezier = [&](double t) -> XYPos {
        double u = 1.0 - t;
        double uu = u * u;
        double uuu = uu * u;
        double tt = t * t;
        double ttt = tt * t;

        XYPos p;
        p.x_mm = uuu * startPos.x_mm
                 + 3 * uu * t * controlPoint1.x_mm
                 + 3 * u * tt * controlPoint2.x_mm
                 + ttt * targetPos.x_mm;

        p.y_mm = uuu * startPos.y_mm
                 + 3 * uu * t * controlPoint1.y_mm
                 + 3 * u * tt * controlPoint2.y_mm
                 + ttt * targetPos.y_mm;

        return p;
    };

    const double maxSegmentLength = _min_feature_size_mm;

    double t0 = 0.0;
    XYPos lastPos = startPos;

    // Fully adaptive loop
    while (t0 < 1.0) {
        // Small initial increment in t
        double dt = 0.01;
        double t1 = t0 + dt;
        if (t1 > 1.0) t1 = 1.0;

        XYPos p1 = bezier(t1);

        // Increase t1 until distance >= maxSegmentLength or t1 reaches 1.0
        while (t1 < 1.0) {
            double dx = p1.x_mm - lastPos.x_mm;
            double dy = p1.y_mm - lastPos.y_mm;
            double dist = std::sqrt(dx*dx + dy*dy);
            if (dist >= maxSegmentLength) break;

            t1 += dt;
            if (t1 > 1.0) t1 = 1.0;
            p1 = bezier(t1);
        }

        // Move the pen to the computed point
        moveToXY(p1, mm_per_s);

        // Prepare for next segment
        lastPos = p1;
        t0 = t1;
    }

    // Ensure the pen ends exactly at target
    moveToXY(targetPos, mm_per_s);
}