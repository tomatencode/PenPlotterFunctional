#pragma once
#include <Arduino.h>
#include <FS.h>

#include "systemServices/shared/SharedData.hpp"

struct PlotJob {
    File file = File();
    uint32_t totalLines = 0;
    uint32_t currentBufferLine = 0;
};

class JobManager {
public:
    JobManager();

    void start(String filename);
    void pause();
    void resume();
    void abort();

    bool isActive() const { return _active; }
    bool isJobPaused() const;
    double currentProgress() const;

    uint32_t getCurrentBufferLine() const { return currentJob.currentBufferLine; }
    uint16_t getCurrentLine() const { return telemetry.currentLineNumber; }
    uint32_t getTotalLines() const { return currentJob.totalLines; }

    void jobManagerUpdate();
private:
    PlotJob currentJob;
    bool _active;
};