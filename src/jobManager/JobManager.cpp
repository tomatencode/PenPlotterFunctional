#include "JobManager.hpp"

#include <FS.h>

#include "storage/FileSystem.hpp"
#include "systemServices/Queues.hpp"


JobManager::JobManager(MotionStateManager& ms) : currentJob(PlotJob()), _active(false), ms(ms) {}

void JobManager::start(String filename)
{
    abort(); // Ensure any existing job is stopped before starting a new one

    Serial.println("Starting job: " + filename);

    ms.setCommand(MotionCommand::NONE); // Clear any existing motion commands
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
    ms.setCommand(MotionCommand::PAUSE);
}

void JobManager::resume()
{
    Serial.println("Resuming job");
    ms.setCommand(MotionCommand::NONE);
}

void JobManager::abort()
{
    Serial.println("Aborting job");
    ms.setCommand(MotionCommand::ABORT);
    xQueueReset(gcodeQueue); // Clear any pending G-code commands
    if (currentJob.file) currentJob.file.close();
    currentJob.currentBufferLine = 0;
    _active = false;
}

bool JobManager::isJobPaused() const
{
    return ms.getCommand() == MotionCommand::PAUSE;
}

uint16_t JobManager::getCurrentLine() const
{
    return currentJob.currentBufferLine - uxQueueMessagesWaiting(gcodeQueue);
}

void JobManager::update()
{
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

        line.toCharArray(msg.line, MAX_GCODE_LINE);
        msg.line[MAX_GCODE_LINE - 1] = '\0';   // safety termination

        if (xQueueSend(gcodeQueue, &msg, 0) != pdTRUE)
        {
            // queue full, stop sending
            break;
        }
    }
}