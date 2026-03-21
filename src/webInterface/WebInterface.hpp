#pragma once

#include <WebServer.h>
#include <cstdint>

#include "jobManager/JobManager.hpp"

class WebInterface
{
public:
    WebInterface(JobManager& jobManager);
    void init();
    void update();
    bool isWiFiConnected() const;

private:
    JobManager& _jobManager;

    WebServer server;
    bool _wifiInitialized = false;
    bool _serverStarted = false;
    unsigned long _lastWiFiCheck = 0;
    unsigned long _wifiStartTime = 0;
    static const unsigned long WIFI_CONNECT_TIMEOUT = 30000; // 30 seconds max

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