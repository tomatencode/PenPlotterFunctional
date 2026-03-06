#include <Arduino.h>

#include "systemServices/Queues.hpp"
#include "systemServices/Tasks.hpp"

void setup()
{
    initQueues();      // communication queues
    startSystemTasks();// multicore tasks
}

void loop() {}