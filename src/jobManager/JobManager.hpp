#pragma once
#include <Arduino.h>
#include <FS.h>
#include <vector>

#include "storage/FileManager.hpp"
#include "systemServices/MotionState.hpp"
#include "JobObserver.hpp"

struct PlotJob {
    File file = File();
    uint32_t totalLines = 0;
    uint32_t currentBufferLine = 0;
    String filename = "";
    bool completed = false;
};

class JobManager {
public:
    JobManager(MotionState& ms, FileManager& fileManager)
        : currentJob(PlotJob()), _active(false), ms(ms), fileManager(fileManager)
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
    String getCurrentFile() const { return currentJob.filename; }
    uint32_t getTotalLines() const { return currentJob.totalLines; }
    uint16_t getCurrentLine() const;

    void update();

private:
    PlotJob currentJob;
    bool _active;
    std::vector<JobObserver*> _observers;

    MotionState& ms;
    FileManager& fileManager;

    // Internal notification method
    void notifyObservers(JobEvent event);
};