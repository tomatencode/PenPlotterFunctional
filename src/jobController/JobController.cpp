#include "JobController.hpp"
#include "config/DirectoriesConfig.hpp"

#include <FS.h>
#include <cstring>
#include <esp_log.h>

static const char* TAG = "JobController";

void JobController::start(const std::string& filename)
{
    abort(); // Ensure any existing job is stopped before starting a new one

    _gcodeToken = _gcodeSender.tryAcquire();
    if (!_gcodeToken) {
        ESP_LOGW(TAG, "Cannot start job - GCode sender is busy");
        return;
    }

    ESP_LOGI(TAG, "Starting job: %s", filename.c_str());

    _motionState.setCommand(MotionCommand::NONE); // Clear any existing motion commands
    _currentJob.file = _fileManager.openFileRead(PLOTTING_DIRECTORY + filename);

    if (!_currentJob.file)
    {
        ESP_LOGE(TAG, "Failed to open file: %s", filename.c_str());
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

    _buzzer.playMelody(_jobStartMelody);
}

void JobController::pause()
{
    if (!_active) return;

    _buzzer.playMelody(_jobPauseMelody);
    
    ESP_LOGI(TAG, "Pausing job");
    _motionState.setCommand(MotionCommand::PAUSE);
    notifyObservers({.type = JobEvent::PAUSED, .filename = _currentJob.filename});
}

void JobController::resume()
{
    if (!_active) return;

    _buzzer.playMelody(_jobResumeMelody);

    ESP_LOGI(TAG, "Resuming job");
    _motionState.setCommand(MotionCommand::NONE);
    notifyObservers({.type = JobEvent::RESUMED, .filename = _currentJob.filename});
}

void JobController::abort()
{
    if (!_active) return;

    _buzzer.playMelody(_jobAbortMelody);

    ESP_LOGI(TAG, "Aborting job");
    _motionState.setCommand(MotionCommand::ABORT);
    _gcodeToken->clearQueue();

    std::string filename = _currentJob.filename; // Capture filename before ending job
    endCurrentJob();

    notifyObservers({.type = JobEvent::ABORTED, .filename = filename});
}

void JobController::update()
{
    if (!_active) return;

    // Check for completion
    if (!_currentJob.file.available() && _gcodeToken->messagesWaiting() == 0) {
        std::string filename = _currentJob.filename; // Capture filename before ending job
        endCurrentJob();
        _buzzer.playMelody(_jobCompleteMelody);
        notifyObservers({.type = JobEvent::COMPLETED, .filename = filename});
        return;
    }
    
    if (!_currentJob.file.available()) return; // No more lines to read

    UBaseType_t space = _gcodeToken->spacesAvailable();
    for (UBaseType_t i = 0; i < space && _currentJob.file.available(); i++)
    {
        String line = _currentJob.file.readStringUntil('\n');
        std::string lineStr(line.c_str());
        // trim whitespace and carriage returns
        lineStr.erase(0, lineStr.find_first_not_of(" \t\r\n"));
        if (!lineStr.empty()) lineStr.erase(lineStr.find_last_not_of(" \t\r\n") + 1);

        _currentJob.currentBufferLine++;

        if (lineStr.length() == 0) continue; // skip empty lines

        if (!_gcodeToken->send(lineStr)) break; // queue full
    }
}

void JobController::endCurrentJob()
{
    _gcodeToken = std::nullopt; // RAII destructor releases the lock

    _active = false;
    if (_currentJob.file) {
        _currentJob.file.close();
    }
    _currentJob = PlotJob();
    ESP_LOGI(TAG, "Job ended");
}


uint32_t JobController::getCurrentLine() const
{
    if (!_active || !_gcodeToken) return 0;
    size_t linesInQueue = _gcodeToken->messagesWaiting();
    if (linesInQueue > _currentJob.currentBufferLine) return 0;
    return _currentJob.currentBufferLine - linesInQueue;
}

double JobController::getProgress() const
{
    if (!_active || _currentJob.totalLines == 0) return 0.0;
    return static_cast<double>(getCurrentLine()) / _currentJob.totalLines;
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