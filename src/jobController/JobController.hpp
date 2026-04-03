#pragma once
#include <Arduino.h>
#include <FS.h>
#include <string>
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
    std::string filename = "";
};

class JobController {
public:
    JobController(MotionState& motionState, FreeRtosQueue<GcodeMessage>& gcodeQueue, FileManager& fileManager, std::string plottingDirectory)
        : _currentJob(PlotJob()), _active(false), _motionState(motionState), _gcodeQueue(gcodeQueue), _fileManager(fileManager), _plottingDirectory(std::move(plottingDirectory))
    {}

    // Job control
    void start(const std::string& filename);
    void pause();
    void resume();
    void abort();

    // Observer management
    void registerObserver(JobObserver* observer);
    void unregisterObserver(JobObserver* observer);

    // Status queries
    bool isActive() const { return _active; }
    std::string getCurrentFile() const { return _currentJob.filename; }
    uint32_t getTotalLines() const { return _currentJob.totalLines; }
    uint16_t getCurrentLine() const;

    void update();

private:
    PlotJob _currentJob;
    bool _active;
    std::string _plottingDirectory;

    MotionState& _motionState;
    FileManager& _fileManager;
    FreeRtosQueue<GcodeMessage>& _gcodeQueue;

    std::vector<JobObserver*> _observers;

    void endCurrentJob();

    // Internal notification method
    void notifyObservers(const JobEventType& event);
};