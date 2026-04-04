#pragma once

#include <WebServer.h>

#include "systemServices/MotionState.hpp"
#include "storage/FileManager.hpp"
#include "jobController/JobController.hpp"
#include "settings/SettingsObserver.hpp"
#include "settings/SettingsRepository.hpp"

class WebInterface : public SettingsObserver
{
public:
    WebInterface(JobController& jobController, MotionState& motionState, FileManager& fileManager, SettingsRepository& settingsRepository);
    
    ~WebInterface() = default;

    void init();
    void update();
    bool isWiFiConnected() const;

private:
    JobController& _jobController;
    MotionState& _motionState;
    FileManager& _fileManager;
    SettingsRepository& _settingsRepository;

    WebServer _server; // HTTP server (port 80)
    bool _serverStarted = false;

    void startWiFiConnection();
    void setupServer();

    // HTTP Handlers
    void handleFileList();
    void handleStartJob();
    void handleAbortJob();
    void handlePauseJob();
    void handleResumeJob();
    void handleUpload();

    void onNetworkSettingsChanged(const NetworkSettings& newSettings) override;
};