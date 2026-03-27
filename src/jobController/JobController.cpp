#include "JobController.hpp"

#include <FS.h>

void JobController::start(String filename)
{
    abort(); // Ensure any existing job is stopped before starting a new one

    Serial.println("Starting job: " + filename);

    _motionState.setCommand(MotionCommand::NONE); // Clear any existing motion commands
    _currentJob.file = _fileManager.openFileRead(_plottingDirectory + "/" + filename);

    if (!_currentJob.file)
    {
        Serial.println("Failed to open file");
        return;
    }

    _currentJob.filename = filename;
    _currentJob.completed = false;
    _currentJob.totalLines = 0;
    _currentJob.currentBufferLine = 0;
    _active = true;

    // Notify observers as fast as possible
    notifyObservers({.type = JobEvent::STARTED, .filename = _currentJob.filename});

    // Count total lines for progress tracking
    while (_currentJob.file.available())
    {
        _currentJob.file.readStringUntil('\n');
        _currentJob.totalLines++;
    }

    // Close and reopen to reset read position
    _currentJob.file.close();
    _currentJob.file = _fileManager.openFileRead(_plottingDirectory + "/" + filename);
}

void JobController::pause()
{
    Serial.println("Pausing job");
    _motionState.setCommand(MotionCommand::PAUSE);
    notifyObservers({.type = JobEvent::PAUSED, .filename = _currentJob.filename});
}

void JobController::resume()
{
    Serial.println("Resuming job");
    _motionState.setCommand(MotionCommand::NONE);
    notifyObservers({.type = JobEvent::RESUMED, .filename = _currentJob.filename});
}

void JobController::abort()
{
    if (!_active) return; // No active job to abort

    Serial.println("Aborting job");
    _motionState.setCommand(MotionCommand::ABORT);
    _gcodeQueue.clear(); // Clear any pending G-code commands
    if (_currentJob.file) _currentJob.file.close();
    _currentJob.currentBufferLine = 0;
    _active = false;
    _currentJob.completed = false;
    notifyObservers({.type = JobEvent::ABORTED, .filename = _currentJob.filename});
}

uint16_t JobController::getCurrentLine() const
{
    return _currentJob.currentBufferLine - _gcodeQueue.messagesWaiting();
}

void JobController::update()
{
    if (!_active) return; // No active job
    
    // Check for completion
    if (!_currentJob.file.available() && !_currentJob.completed) {
        if (_gcodeQueue.messagesWaiting() == 0) {
            // File is done and queue is empty - job is complete
            _currentJob.completed = true;
            _active = false;
            if (_currentJob.file) _currentJob.file.close();
            notifyObservers({.type = JobEvent::COMPLETED, .filename = _currentJob.filename});
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

void JobController::registerObserver(JobObserver* observer)
{
    if (observer == nullptr) return;
    
    // Avoid duplicate registrations
    for (auto obs : _observers) {
        if (obs == observer) return;
    }
    
    _observers.push_back(observer);
    Serial.println("Observer registered. Total observers: " + String(_observers.size()));
}

void JobController::unregisterObserver(JobObserver* observer)
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

void JobController::notifyObservers(const JobEventType& event)
{
    for (auto observer : _observers) {
        if (observer != nullptr) {
            observer->onJobEvent(event);
        }
    }
}