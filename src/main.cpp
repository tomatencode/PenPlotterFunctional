#include <Arduino.h>

#include "shared/Queues.hpp"
#include "system/Tasks.hpp"

void setup()
{
    initQueues();      // communication queues
    startSystemTasks();// multicore tasks
}

void loop() {}