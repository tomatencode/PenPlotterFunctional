#include "JobManager.hpp"

#include <FS.h>

#include "../storage/FileSystem.hpp"
#include "../system/Queues.hpp"

static File currentFile;
static bool running = false;
static uint32_t currentLineIndex = 0;

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

    currentLineIndex = 0;
    running = true;
}

void jobStop()
{
    if (currentFile)
        currentFile.close();

    currentLineIndex = 0;
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

        // skip empty lines
        if (line.length() == 0)
            continue;

        GcodeMessage msg;

        msg.lineNumber = currentLineIndex++;   // track job progress

        line.toCharArray(msg.line, MAX_GCODE_LINE);
        msg.line[MAX_GCODE_LINE - 1] = '\0';   // safety termination

        if (xQueueSend(gcodeQueue, &msg, 0) != pdTRUE)
        {
            // queue full, stop sending
            break;
        }
    }
}