#include "JobManager.hpp"

#include <FS.h>

void JobManager::start(String filename)
{
    abort(); // Ensure any existing job is stopped before starting a new one

    Serial.println("Starting job: " + filename);

    _motionState.setCommand(MotionCommand::NONE); // Clear any existing motion commands
    _currentJob.file = _fileManager.openFileRead(filename);

    if (!_currentJob.file)
    {
        Serial.println("Failed to open file");
        return;
    }

    _currentJob.filename = filename;
    _currentJob.completed = false;
    _currentJob.totalLines = 0;
    while (_currentJob.file.available())
    {
        _currentJob.file.readStringUntil('\n'); // Read and discard lines to count them
        _currentJob.totalLines++;
    }

    // Close and reopen to reset read position
    _currentJob.file.close();
    _currentJob.file = _fileManager.openFileRead(filename);

    _currentJob.currentBufferLine = 0;
    _active = true;

    // Notify observers that a job has started
    notifyObservers(JobEvent::STARTED);
}

void JobManager::pause()
{
    Serial.println("Pausing job");
    _motionState.setCommand(MotionCommand::PAUSE);
    notifyObservers(JobEvent::PAUSED);
}

void JobManager::resume()
{
    Serial.println("Resuming job");
    _motionState.setCommand(MotionCommand::NONE);
    notifyObservers(JobEvent::RESUMED);
}

void JobManager::abort()
{
    Serial.println("Aborting job");
    _motionState.setCommand(MotionCommand::ABORT);
    _gcodeQueue.clear(); // Clear any pending G-code commands
    if (_currentJob.file) _currentJob.file.close();
    _currentJob.currentBufferLine = 0;
    _active = false;
    _currentJob.completed = false;
    notifyObservers(JobEvent::ABORTED);
}

uint16_t JobManager::getCurrentLine() const
{
    return _currentJob.currentBufferLine - _gcodeQueue.messagesWaiting();
}

void JobManager::update()
{
    if (!_active) return; // No active job
    
    // Check for completion
    if (!_currentJob.file.available() && !_currentJob.completed) {
        if (_gcodeQueue.messagesWaiting() == 0) {
            // File is done and queue is empty - job is complete
            _currentJob.completed = true;
            _active = false;
            if (_currentJob.file) _currentJob.file.close();
            notifyObservers(JobEvent::COMPLETED);
            return;
        }
    }
    
    if (!_currentJob.file.available()) return; // No more lines to read

    // prefetch queue space
    UBaseType_t space = _gcodeQueue.spacesAvailable();

    for (UBaseType_t i = 0; i < space && _currentJob.file.available(); i++)
    {
        String line = _currentJob.file.readStringUntil('\n');
        line.trim();

        _currentJob.currentBufferLine++;

        // skip empty lines
        if (line.length() == 0)
            continue;

        GcodeMessage msg;

        line.toCharArray(msg.line, MAX_GCODE_LINE);
        msg.line[MAX_GCODE_LINE - 1] = '\0';   // safety termination

        if (_gcodeQueue.trySend(msg, 0))
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

void JobManager::notifyObservers(JobEvent event)
{
    for (auto observer : _observers) {
        if (observer != nullptr) {
            observer->onJobEvent(event);
        }
    }
}