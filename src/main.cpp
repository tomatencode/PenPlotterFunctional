#include <Arduino.h>

#include "system/Machine.hpp"
#include "system/Queues.hpp"
#include "system/Tasks.hpp"

void setup()
{
    initQueues();      // communication queues
    startSystemTasks();// multicore tasks
}

void loop() {}