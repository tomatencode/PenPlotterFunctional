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
          _settingPercistence(settingsRepository),
          _runtimeSettings(runtimeSettings),
          _server(80)
    {}
    
    ~WebInterface() = default;

    void init();
    void update();
    bool isWiFiConnected() const;

private:
    JobController& _jobController;
    MotionState& _motionState;
    FileManager& _fileManager;
    SettingPersistence& _settingPercistence;
    RuntimeSettings& _runtimeSettings;

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

    void onRelevantSettingsChanged() override;
};