#pragma once
#include <Arduino.h>
#include <FS.h>
#include <vector>

#include "storage/FileManager.hpp"
#include "systemServices/MotionState.hpp"
#include "systemServices/FreeRtosQueue.hpp"
#include "systemServices/GcodeMessage.hpp"
#include "JobObserver.hpp"

struct PlotJob {
    File file = File();
    uint32_t totalLines = 0;
    uint32_t currentBufferLine = 0;
    String filename = "";
    bool completed = false;
};

class JobController {
public:
    JobController(MotionState& ms, FreeRtosQueue<GcodeMessage>& gcodeQueue, FileManager& fileManager)
        : _currentJob(PlotJob()), _active(false), _motionState(ms), _gcodeQueue(gcodeQueue), _fileManager(fileManager)
    {}

    // Job control
    void start(String filename);
    void pause();
    void resume();
    void abort();

    // Observer management
    void registerObserver(JobObserver* observer);
    void unregisterObserver(JobObserver* observer);

    // Status queries
    bool isActive() const { return _active; }
    String getCurrentFile() const { return _currentJob.filename; }
    uint32_t getTotalLines() const { return _currentJob.totalLines; }
    uint16_t getCurrentLine() const;

    void update();

private:
    PlotJob _currentJob;
    bool _active;
    std::vector<JobObserver*> _observers;

    MotionState& _motionState;
    FileManager& _fileManager;
    FreeRtosQueue<GcodeMessage>& _gcodeQueue;

    // Internal notification method
    void notifyObservers(JobEvent event);
};