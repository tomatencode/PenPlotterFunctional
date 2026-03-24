#pragma once
#include <Arduino.h>
#include <FS.h>

#include "systemServices/MotionStateManager.hpp"

struct PlotJob {
    File file = File();
    uint32_t totalLines = 0;
    uint32_t currentBufferLine = 0;
};

class JobManager {
public:
    JobManager(MotionStateManager& ms);

    void start(String filename);
    void pause();
    void resume();
    void abort();

    bool isActive() const { return _active; }
    bool isJobPaused() const;

    uint32_t getTotalLines() const { return currentJob.totalLines; }
    uint16_t getCurrentLine() const;

    void update();
private:
    PlotJob currentJob;
    bool _active;

    MotionStateManager& ms;
};