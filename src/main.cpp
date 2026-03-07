#include <Arduino.h>

#include "systemServices/Queues.hpp"
#include "systemServices/Tasks.hpp"

void setup()
{
    Serial.begin(115200); // Initialize serial communication for debugging
    initQueues();         // communication queues
    startSystemTasks();   // multicore tasks
}

void loop() {}