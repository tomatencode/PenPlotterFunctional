#include <Arduino.h>

#include "system/Machine.hpp"
#include "system/Queues.hpp"
#include "system/Tasks.hpp"

void setup()
{
    initMachine();     // hardware + motion system
    initQueues();      // communication queues
    startSystemTasks();// multicore tasks
}

void loop() {}