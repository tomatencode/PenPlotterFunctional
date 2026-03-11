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

    void jobManagerUpdate();
private:
    File currentFile;
    uint32_t currentLineIndex;
};