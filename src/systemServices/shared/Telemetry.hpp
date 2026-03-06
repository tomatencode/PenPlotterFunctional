#pragma once
#include <cstdint>

enum class MotionState
{
    IDLE = 0,
    RUNNING = 1,
    PAUSED = 2
};

struct Telemetry
{
    float machineX;
    float machineY;

    uint32_t currentLineNumber;

    MotionState state;
};