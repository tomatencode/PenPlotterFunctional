#include "JobManager.hpp"

#include <FS.h>

#include "../storage/FileSystem.hpp"
#include "../system/Queues.hpp"

static File currentFile;
static bool running = false;

#define MAX_GCODE_LINE 96

void jobManagerInit() {}

void jobStart(String filename)
{
    if (running)
        jobStop();

    currentFile = fsOpenRead(filename);

    if (!currentFile)
    {
        Serial.println("Failed to open file");
        return;
    }

    running = true;
}

void jobStop()
{
    if (currentFile)
        currentFile.close();

    running = false;
}

bool jobRunning()
{
    return running;
}

void jobManagerUpdate()
{
    if (!running)
        return;

    if (!currentFile.available())
    {
        Serial.println("Job finished");
        jobStop();
        return;
    }

    // prefetch queue space
    UBaseType_t space = uxQueueSpacesAvailable(gcodeQueue);

    for (UBaseType_t i = 0; i < space && currentFile.available(); i++)
    {
        String line = currentFile.readStringUntil('\n');
        line.trim();

        // skip empty lines instead of breaking
        if (line.length() == 0) continue;

        char buffer[MAX_GCODE_LINE];
        line.toCharArray(buffer, MAX_GCODE_LINE);

        if (xQueueSend(gcodeQueue, &buffer, 0) != pdTRUE)
        {
            // queue full, stop sending
            break;
        }
    }
}