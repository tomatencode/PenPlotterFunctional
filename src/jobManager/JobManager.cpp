#include "JobManager.hpp"

#include <FS.h>

#include "../storage/FileSystem.hpp"
#include "../systemServices/Queues.hpp"
#include "../systemServices/shared/SharedData.hpp"
#include "../systemServices/shared/MotionCommand.hpp"
#include "../systemServices/shared/Telemetry.hpp"


JobManager::JobManager() : currentLineIndex(0) {}

void JobManager::start(String filename)
{
    currentFile = fsOpenRead(filename);

    if (!currentFile)
    {
        Serial.println("Failed to open file");
        return;
    }

    currentLineIndex = 0;
}

void JobManager::pause()
{
    motionCommand = MotionCommand::PAUSE;
}

void JobManager::resume()
{
    motionCommand = MotionCommand::NONE;
}

void JobManager::abort()
{
    motionCommand = MotionCommand::ABORT;
    xQueueReset(gcodeQueue); // Clear any pending G-code commands
    if (currentFile) currentFile.close();
    currentLineIndex = 0;
}


void JobManager::jobManagerUpdate()
{
    // If an abort command was issued and the machine is now idle, clear the abort command
    if (motionCommand == MotionCommand::ABORT && telemetry.state == MotionState::IDLE)
    {
        motionCommand = MotionCommand::NONE;
    }

    if (!currentFile || !currentFile.available())
    {
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