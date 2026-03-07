// SharedData.cpp
#include "shared/SharedData.hpp"

// Core1 writes telemetry
volatile Telemetry telemetry = {0.0f, 0.0f, 0, MotionState::IDLE};

// Core0 writes commands
volatile MotionCommand motionCommand = MotionCommand::NONE;