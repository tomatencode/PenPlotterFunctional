#include "JobManager.hpp"

#include <FS.h>

#include "storage/FileSystem.hpp"
#include "systemServices/Queues.hpp"
#include "systemServices/shared/SharedData.hpp"
#include "systemServices/shared/MotionCommand.hpp"
#include "systemServices/shared/Telemetry.hpp"


JobManager::JobManager() : currentJob(PlotJob()), _active(false) {}

void JobManager::start(String filename)
{
    abort(); // Ensure any existing job is stopped before starting a new one

    Serial.println("Starting job: " + filename);

    motionCommand = MotionCommand::NONE; // Clear any existing motion commands
    currentJob.file = storage::fsOpenRead(filename);

    if (!currentJob.file)
    {
        Serial.println("Failed to open file");
        return;
    }

    currentJob.totalLines = 0;
    while (currentJob.file.available())
    {
        currentJob.file.readStringUntil('\n'); // Read and discard lines to count them
        currentJob.totalLines++;
    }

    // Close and reopen to reset read position
    currentJob.file.close();
    currentJob.file = storage::fsOpenRead(filename);

    currentJob.currentBufferLine = 0;
    _active = true;
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
    if (currentJob.file) currentJob.file.close();
    currentJob.currentBufferLine = 0;
    _active = false;
}

bool JobManager::isJobPaused() const
{
    return motionCommand == MotionCommand::PAUSE;
}

double JobManager::currentProgress() const
{
    if (!currentJob.file || currentJob.totalLines == 0)
        return 0.0;

    return static_cast<double>(telemetry.currentLineNumber) / static_cast<double>(currentJob.totalLines);
}

void JobManager::jobManagerUpdate()
{
    // If an abort command was issued and the machine is now idle, clear the abort command
    if (motionCommand == MotionCommand::ABORT && telemetry.state == MotionState::IDLE)
    {
        motionCommand = MotionCommand::NONE;
    }

    if (!_active) return; // No active job
    if (!currentJob.file.available()) return; // No more lines to read

    // prefetch queue space
    UBaseType_t space = uxQueueSpacesAvailable(gcodeQueue);

    for (UBaseType_t i = 0; i < space && currentJob.file.available(); i++)
    {
        String line = currentJob.file.readStringUntil('\n');
        line.trim();

        currentJob.currentBufferLine++;

        // skip empty lines
        if (line.length() == 0)
            continue;

        GcodeMessage msg;
        msg.lineNumber = currentJob.currentBufferLine;

        line.toCharArray(msg.line, MAX_GCODE_LINE);
        msg.line[MAX_GCODE_LINE - 1] = '\0';   // safety termination

        if (xQueueSend(gcodeQueue, &msg, 0) != pdTRUE)
        {
            // queue full, stop sending
            break;
        }
    }
}