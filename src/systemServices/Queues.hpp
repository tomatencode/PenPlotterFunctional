#pragma once
#include <Arduino.h>

#define MAX_GCODE_LINE 96

extern QueueHandle_t gcodeQueue;

struct GcodeMessage
{
    char line[MAX_GCODE_LINE];
};

void initQueues();