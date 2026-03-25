#pragma once
#include <cstddef>

static constexpr size_t MAX_GCODE_LINE = 96;

struct GcodeMessage {
    char line[MAX_GCODE_LINE];
};