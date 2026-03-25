#include <Arduino.h>

#include "systemServices/Tasks.hpp"

void setup()
{
    Serial.begin(115200);
    startSystemTasks();
}

void loop() {}