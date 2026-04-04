#pragma once

#include "StepperAxis.hpp"
#include "systemServices/MotionState.hpp"

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

    // Compile-time homing constants (not meant to be dynamic)
    static constexpr float _sgCheckInterval_ms = 50.0f;
    static constexpr uint16_t _consecutiveStallChecks = 5;
    static constexpr uint16_t _sgStartTimeout_ms = 500;

    void moveToLimit(bool Afw, bool Bfw);
};