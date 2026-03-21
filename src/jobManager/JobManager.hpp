#pragma once
#include <Arduino.h>
#include <FS.h>

class JobManager {
public:
    JobManager();

    void start(String filename);
    void pause();
    void resume();
    void abort();

    bool isJobPaused() const;
    double currentProgress() const;

    void jobManagerUpdate();
private:
    File currentFile;
    uint32_t currentLineIndex;
};