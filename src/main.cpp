#include <Arduino.h>

#include "system/Machine.hpp"
#include "system/Queues.hpp"
#include "system/Tasks.hpp"

extern std::vector<String> gcodeLines = {
    // Homing sequence
    "G28", // Home all axes

    // Head (circle with radius 40mm, centered at 50,50)
    "M5",
    "G0 X60 Y100",          // Move to leftmost point of head
    "M3",
    "G2 X60 Y100 I40 J0 F20", // Clockwise full circle (head)
    "M5",

    // Left eye (circle radius 5mm)
    "G0 X80 Y115",          // Move to left eye
    "M3",
    "G2 X80 Y115 I5 J0 F15",   // small circle for left eye
    "M5",

    // Right eye (circle radius 5mm)
    "G0 X110 Y115",          // Move to right eye
    "M3",
    "G2 X110 Y115 I5 J0 F15",   // small circle for right eye
    "M5",

    // Smile (quadratic Bézier)
    "G0 X80 Y85",          // Start of smile
    "M3",
    "G5 X120 Y85 C100 D70 F15", // Control point at (50,20), target at (70,35)
    "M5",

    // Move out of the way
    "G0 X0 Y0 F50"
};

void setup()
{
    initMachine();     // hardware + motion system
    initQueues();      // communication queues
    startSystemTasks();// multicore tasks
}

void loop() {}