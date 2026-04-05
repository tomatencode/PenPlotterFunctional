#pragma once

#include "../motionExecution/StepperAxis.hpp"
#include "rtos/MotionState.hpp"

// Forward declaration
class RuntimeSettings;

class HomingController {
public:
    HomingController(StepperAxis& axisA, StepperAxis& axisB, MotorDriver& driverA, MotorDriver& driverB, MotionState& motionState, RuntimeSettings& runtimeSettings);

    void home();

private:
    StepperAxis& _axisA;
    StepperAxis& _axisB;
    MotorDriver& _driverA;
    MotorDriver& _driverB;
    MotionState& _motionState;
    RuntimeSettings& _runtimeSettings;

    void moveToLimit(bool Afw, bool Bfw);
    bool checkPauseAbort();  // Handle pause/abort during homing; return true if should abort
};