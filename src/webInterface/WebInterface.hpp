#pragma once

#include <WebServer.h>
#include <cstdint>

#include "systemServices/MotionState.hpp"
#include "storage/FileManager.hpp"
#include "jobManager/JobManager.hpp"

class WebInterface
{
public:
    WebInterface(JobManager& jobManager, MotionState& ms, FileManager& fileManager)
        : _jobManager(jobManager), _ms(ms), _fileManager(fileManager), server(80)
    {}

    void init();
    void update();
    bool isWiFiConnected() const;

private:
    JobManager& _jobManager;
    MotionState& _ms;
    FileManager& _fileManager;


    WebServer server;
    bool _wifiInitialized = false;
    bool _serverStarted = false;


    void handleFileList();
    void handlePauseJob();
    void handleResumeJob();
    void handleStartJob();
    void handleAbortJob();
    void handleUpload();
    
    void startWiFiConnection();
    void checkWiFiStatus();
    void setupServer();
};