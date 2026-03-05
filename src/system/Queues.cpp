#include "Queues.hpp"

QueueHandle_t gcodeQueue;

void initQueues()
{
    gcodeQueue = xQueueCreate(32, MAX_GCODE_LINE);
}