#pragma once
#include <Arduino.h>
#include <FS.h>
#include <vector>

#include "systemServices/MotionStateManager.hpp"
#include "JobObserver.hpp"
#include "JobStatusUpdate.hpp"

struct PlotJob {
    File file = File();
    uint32_t totalLines = 0;
    uint32_t currentBufferLine = 0;
    String filename = "";
    bool completed = false;
};

class JobManager {
public:
    JobManager(MotionStateManager& ms);

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

    MotionStateManager& ms;

    // Internal notification method
    void notifyObservers(JobEventType eventType);
};