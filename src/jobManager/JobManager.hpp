#ifndef JobManager_HPP
#define JobManager_HPP
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

#endif