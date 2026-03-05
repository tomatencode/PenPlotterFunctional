#include "Queues.hpp"

QueueHandle_t gcodeQueue;

void initQueues()
{
    gcodeQueue = xQueueCreate(32, sizeof(GcodeMessage));
}