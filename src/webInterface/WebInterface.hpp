#pragma once

#include <WebServer.h>

#include "rtos/MotionState.hpp"
#include "storage/FileManager.hpp"
#include "jobController/JobController.hpp"
#include "settings/SettingObserver.hpp"
#include "settings/SettingPersistence.hpp"
#include "settings/RuntimeSettings.hpp"

class WebInterface : public SettingObserver
{
public:
    WebInterface(JobController& jobController,
                 MotionState& motionState,
                 FileManager& fileManager,
                 SettingPersistence& settingsRepository,
                 RuntimeSettings& runtimeSettings)

        : SettingObserver({Setting::SSID, Setting::Password, Setting::MdnsName}),
          _jobController(jobController),
          _motionState(motionState),
          _fileManager(fileManager),
          _settingPersistence(settingsRepository),
          _runtimeSettings(runtimeSettings),
          _server(80)
    {}
    
    ~WebInterface();

    void init();
    void update();
    bool isWiFiConnected() const;

private:
    JobController& _jobController;
    MotionState& _motionState;
    FileManager& _fileManager;
    SettingPersistence& _settingPersistence;
    RuntimeSettings& _runtimeSettings;

    WebServer _server; // HTTP server (port 80)
    bool _serverStarted = false;

    bool _WifiConfigured = false;
    uint32_t _lastWiFiConnectAttemptMs = 0;
    uint32_t _wifiReconnectIntervalMs = 5000;
    uint32_t _WifiConfigureTimeoutMs = 100; // time to wait after configureing wifi
    uint16_t _maxWiFiConnectAttempts = 10;
    uint16_t _currentWiFiConnectAttempts = 0;

    void configureWifi();
    void attemptWiFiConnection();
    void setupServer();

    // HTTP Handlers
    void handleFileList();
    void handleStartJob();
    void handleAbortJob();
    void handlePauseJob();
    void handleResumeJob();
    void handleUpload();

    void onRelevantSettingsChanged() override;
};