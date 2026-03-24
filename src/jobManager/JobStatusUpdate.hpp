#pragma once
#include <Arduino.h>
#include <cstdint>

enum class JobEventType {
    STARTED = 0,
    PAUSED = 1,
    RESUMED = 2,
    ABORTED = 3,
    COMPLETED = 4
};

struct JobStatusUpdate {
    JobEventType eventType;

    JobStatusUpdate(JobEventType type)
        : eventType(type)
    {}

};
