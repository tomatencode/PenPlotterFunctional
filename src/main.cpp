#include <Arduino.h>

#include "rtos/Tasks.hpp"

void setup()
{
    Serial.begin(115200);
    startRtosTasks();
}

void loop() {}