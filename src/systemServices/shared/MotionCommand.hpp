#pragma once
#include <cstdint>

enum class MotionCommand : uint8_t
{
    NONE = 0,
    PAUSE = 1,
    ABORT = 2
};