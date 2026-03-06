#pragma once
#include <cstdint>

enum class MotionState
{
    MOTION_IDLE,
    MOTION_RUNNING,
    MOTION_PAUSED
};

struct Telemetry
{
    float machineX;
    float machineY;

    uint32_t currentLineNumber;

    MotionState state;
    bool penDown;
};