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

    currentJob.filename = filename;
    currentJob.completed = false;
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

    // Notify observers that a job has started
    notifyObservers(JobEventType::STARTED);
}

void JobManager::pause()
{
    Serial.println("Pausing job");
    ms.setCommand(MotionCommand::PAUSE);
    notifyObservers(JobEventType::PAUSED);
}

void JobManager::resume()
{
    Serial.println("Resuming job");
    ms.setCommand(MotionCommand::NONE);
    notifyObservers(JobEventType::RESUMED);
}

void JobManager::abort()
{
    Serial.println("Aborting job");
    ms.setCommand(MotionCommand::ABORT);
    xQueueReset(gcodeQueue); // Clear any pending G-code commands
    if (currentJob.file) currentJob.file.close();
    currentJob.currentBufferLine = 0;
    _active = false;
    currentJob.completed = false;
    notifyObservers(JobEventType::ABORTED);
}

uint16_t JobManager::getCurrentLine() const
{
    return currentJob.currentBufferLine - uxQueueMessagesWaiting(gcodeQueue);
}

void JobManager::update()
{
    if (!_active) return; // No active job
    
    // Check for completion
    if (!currentJob.file.available() && !currentJob.completed) {
        if (uxQueueMessagesWaiting(gcodeQueue) == 0) {
            // File is done and queue is empty - job is complete
            currentJob.completed = true;
            _active = false;
            if (currentJob.file) currentJob.file.close();
            notifyObservers(JobEventType::COMPLETED);
            return;
        }
    }
    
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

void JobManager::registerObserver(JobObserver* observer)
{
    if (observer == nullptr) return;
    
    // Avoid duplicate registrations
    for (auto obs : _observers) {
        if (obs == observer) return;
    }
    
    _observers.push_back(observer);
    Serial.println("Observer registered. Total observers: " + String(_observers.size()));
}

void JobManager::unregisterObserver(JobObserver* observer)
{
    if (observer == nullptr) return;
    
    for (size_t i = 0; i < _observers.size(); i++) {
        if (_observers[i] == observer) {
            _observers.erase(_observers.begin() + i);
            Serial.println("Observer unregistered. Total observers: " + String(_observers.size()));
            return;
        }
    }
}

void JobManager::notifyObservers(JobEventType eventType)
{
    JobStatusUpdate update(eventType);
    
    for (auto observer : _observers) {
        if (observer != nullptr) {
            observer->onJobEvent(update);
        }
    }
}