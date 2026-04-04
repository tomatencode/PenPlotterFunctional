#pragma once

#include <WebServer.h>
#include <cstdint>

#include "systemServices/MotionState.hpp"
#include "storage/FileManager.hpp"
#include "jobController/JobController.hpp"
#include "settings/SettingsObserver.hpp"
#include "settings/SettingsRepository.hpp"

class WebInterface : public SettingsObserver
{
public:
    WebInterface(JobController& jobController, MotionState& motionState, FileManager& fileManager, SettingsRepository& settingsRepository)
        : _jobController(jobController), _motionState(motionState), _fileManager(fileManager),
          _settingsRepository(settingsRepository), server(80)
    {}

    void init();
    void update();
    bool isWiFiConnected() const;
    JobController& _jobController;
    MotionState& _motionState;
    FileManager& _fileManager;
    SettingsRepository& _settingsRepository;


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

    // SettingsObserver implementation
    void onNetworkSettingsChanged(const NetworkSettings& newSettings) override;
};