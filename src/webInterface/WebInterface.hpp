#pragma once

#include <WebServer.h>
#include <cstdint>

#include "systemServices/MotionState.hpp"
#include "storage/FileManager.hpp"
#include "jobController/JobController.hpp"

class WebInterface
{
public:
    WebInterface(JobController& jobController, MotionState& motionState, FileManager& fileManager)
        : _jobController(jobController), _motionState(motionState), _fileManager(fileManager), server(80)
    {}

    void init();
    void update();
    bool isWiFiConnected() const;

private:
    JobController& _jobController;
    MotionState& _motionState;
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