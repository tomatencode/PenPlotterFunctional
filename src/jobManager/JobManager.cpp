#include "JobManager.hpp"

#include <FS.h>

#include "storage/FileSystem.hpp"
#include "systemServices/Queues.hpp"
#include "systemServices/shared/SharedData.hpp"
#include "systemServices/shared/MotionCommand.hpp"
#include "systemServices/shared/Telemetry.hpp"


JobManager::JobManager() : currentLineIndex(0) {}

void JobManager::start(String filename)
{
    Serial.println("Starting job: " + filename);

    motionCommand = MotionCommand::NONE; // Clear any existing motion commands
    currentFile = storage::fsOpenRead(filename);

    if (!currentFile)
    {
        Serial.println("Failed to open file");
        return;
    }

    currentLineIndex = 0;
}

void JobManager::pause()
{
    Serial.println("Pausing job");
    motionCommand = MotionCommand::PAUSE;
}

void JobManager::resume()
{
    Serial.println("Resuming job");
    motionCommand = MotionCommand::NONE;
}

void JobManager::abort()
{
    Serial.println("Aborting job");
    motionCommand = MotionCommand::ABORT;
    xQueueReset(gcodeQueue); // Clear any pending G-code commands
    if (currentFile) currentFile.close();
    currentLineIndex = 0;
}

bool JobManager::isJobPaused() const
{
    return motionCommand == MotionCommand::PAUSE;
}

double JobManager::currentProgress() const
{
    if (!currentFile)
        return 0.0;

    size_t totalBytes = currentFile.size();
    size_t currentBytes = currentFile.position();

    Serial.println("Calculating progress: " + String(currentBytes) + " / " + String(totalBytes));

    if (totalBytes == 0)
        return 0.0;

    return static_cast<double>(currentBytes) / static_cast<double>(totalBytes);
}

void JobManager::jobManagerUpdate()
{
    // If an abort command was issued and the machine is now idle, clear the abort command
    if (motionCommand == MotionCommand::ABORT && telemetry.state == MotionState::IDLE)
    {
        motionCommand = MotionCommand::NONE;
    }

    if (!currentFile)
        return;

    if (!currentFile.available())
    {
        // Job finished
        Serial.println("Job completed");
        currentFile.close();
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