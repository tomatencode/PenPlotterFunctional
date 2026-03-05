#pragma once
#include <cstdint>

enum class MotionCommand : uint8_t
{
    NONE,
    PAUSE,
    RESUME,
    STOP
};