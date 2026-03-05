// SharedData.cpp
#include "SharedData.hpp"

// Core1 writes telemetry
volatile Telemetry telemetry;

// Core0 writes commands
volatile MotionCommand motionCommand;