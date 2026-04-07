#include "BezierExecuter.hpp"
#include <cmath>
#include <algorithm>
#include <Arduino.h>

BezierExecuter::BezierExecuter(StepperAxis& axisA, StepperAxis& axisB, CoreXYKinematics& kinematics, MotionState& motionState)
    : _axisA(axisA), _axisB(axisB), _kinematics(kinematics), _motionState(motionState) {}

XYPos BezierExecuter::getCurrentPos() const {
    MotorSteps steps = {_axisA.positionSteps(), _axisB.positionSteps()};
    return _kinematics.stepsToMm(steps);
}

void BezierExecuter::bezierTo(const XYPos& targetPos, double mm_per_s) {
    MotorSteps currentSteps = {_axisA.positionSteps(), _axisB.positionSteps()};
    uint16_t msA = _axisA.microsteps();
    uint16_t msB = _axisB.microsteps();

    XYPos currentPos = _kinematics.stepsToMm(currentSteps);

    double dx = targetPos.xMm - currentPos.xMm;
    double dy = targetPos.yMm - currentPos.yMm;

    double distance_mm = std::sqrt(dx * dx + dy * dy);
    if (distance_mm <= 0.0) return;

    if (mm_per_s <= 0.0) return;

    MotorSteps dfullsteps = _kinematics.mmToSteps({dx, dy});

    int32_t deltaA = static_cast<int32_t>((dfullsteps.a) * msA);
    int32_t deltaB = static_cast<int32_t>((dfullsteps.b) * msB);

    int32_t absA = std::abs(deltaA);
    int32_t absB = std::abs(deltaB);

    bool dirA = (deltaA >= 0);
    bool dirB = (deltaB >= 0);

    int32_t stepsInLoop = std::max(absA, absB);
    if (stepsInLoop == 0) return;

    double move_time_s = distance_mm / mm_per_s;
    double step_frequency = stepsInLoop / move_time_s;
    uint32_t step_interval_us = static_cast<uint32_t>(1e6 / step_frequency);
    if (step_interval_us == 0) step_interval_us = 1;

    uint32_t next_step_time = micros();

    long errA = 0;
    long errB = 0;

    for (int32_t i = 0; i < stepsInLoop; i++) {

        while ((int32_t)(micros() - next_step_time) < 0) {
            yield();
        }

        if (_motionState.getCommand() == MotionCommand::PAUSE) {
            _motionState.setState(MotionStateType::PAUSED);
            while (_motionState.getCommand() == MotionCommand::PAUSE) {
                yield();
            }
            _motionState.setState(MotionStateType::RUNNING);
            next_step_time = micros(); // Reset so pause duration doesn't cause a step burst on resume
        }
        else if (_motionState.getCommand() == MotionCommand::ABORT) {
            return;
        }

        next_step_time += step_interval_us;

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
