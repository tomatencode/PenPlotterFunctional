#include "JobController.hpp"
#include "config/job_config.hpp"

#include <FS.h>
#include <cstring>

void JobController::start(const std::string& filename)
{
    abort(); // Ensure any existing job is stopped before starting a new one

    Serial.println(("Starting job: " + filename).c_str());

    _motionState.setCommand(MotionCommand::NONE); // Clear any existing motion commands
    _currentJob.file = _fileManager.openFileRead(PLOTTING_DIRECTORY + filename);

    if (!_currentJob.file)
    {
        Serial.println("Failed to open file");
        return;
    }

    _currentJob.filename = filename;
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
    _currentJob.file = _fileManager.openFileRead(PLOTTING_DIRECTORY + filename);
}

void JobController::pause()
{
    if (!_active) return; // No active job to pause
    
    Serial.println("Pausing job");
    _motionState.setCommand(MotionCommand::PAUSE);
    notifyObservers({.type = JobEvent::PAUSED, .filename = _currentJob.filename});
}

void JobController::resume()
{
    if (!_active) return; // No active job to resume

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

    endCurrentJob();

    notifyObservers({.type = JobEvent::ABORTED, .filename = _currentJob.filename});
}

uint16_t JobController::getCurrentLine() const
{
    if (!_active) return 0;
    if (_gcodeQueue.messagesWaiting() > _currentJob.currentBufferLine) return 0;
    return _currentJob.currentBufferLine - _gcodeQueue.messagesWaiting();
}

void JobController::update()
{
    if (!_active) return; // No active job
    
    // Check for completion
    if (!_currentJob.file.available() && _gcodeQueue.messagesWaiting() == 0) {
        endCurrentJob();
        notifyObservers({.type = JobEvent::COMPLETED, .filename = _currentJob.filename});
        return;
    }
    
    if (!_currentJob.file.available()) return; // No more lines to read

    // prefetch queue space
    UBaseType_t space = _gcodeQueue.spacesAvailable();

    for (UBaseType_t i = 0; i < space && _currentJob.file.available(); i++)
    {
        String line = _currentJob.file.readStringUntil('\n');
        std::string lineStr(line.c_str());
        // trim whitespace and carriage returns
        lineStr.erase(0, lineStr.find_first_not_of(" \t\r\n"));
        if (!lineStr.empty()) lineStr.erase(lineStr.find_last_not_of(" \t\r\n") + 1);

        _currentJob.currentBufferLine++;

        // skip empty lines
        if (lineStr.length() == 0)
            continue;

        GcodeMessage msg;

        strncpy(msg.line, lineStr.c_str(), MAX_GCODE_LINE - 1);
        msg.line[MAX_GCODE_LINE - 1] = '\0';   // safety termination

        if (_gcodeQueue.trySend(msg, 0))
        {
            // queue full, stop sending
            break;
        }
    }
}

void JobController::endCurrentJob()
{
    _active = false;
    if (_currentJob.file) {
        _currentJob.file.close();
    }
    _currentJob = PlotJob(); // Reset to default
    Serial.println("Job ended");
}

void JobController::registerObserver(JobObserver* observer)
{
    if (observer == nullptr) return;
    
    // Avoid duplicate registrations
    for (auto obs : _observers) {
        if (obs == observer) return;
    }
    
    _observers.push_back(observer);
}

void JobController::unregisterObserver(JobObserver* observer)
{
    if (observer == nullptr) return;
    
    for (size_t i = 0; i < _observers.size(); i++) {
        if (_observers[i] == observer) {
            _observers.erase(_observers.begin() + i);
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