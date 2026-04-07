#pragma once

#include <WebServer.h>

#include "rtos/MotionState.hpp"
#include "storage/FileManager.hpp"
#include "jobController/JobController.hpp"
#include "settings/SettingPersistence.hpp"
#include "settings/RuntimeSettings.hpp"
#include "settings/SettingObserver.hpp"
#include "WifiController.hpp"

class WebInterface : public SettingObserver
{
public:
    WebInterface(JobController& jobController,
                 MotionState& motionState,
                 FileManager& fileManager,
                 WifiController& wifiController,
                 SettingPersistence& settingsPersistence,
                 RuntimeSettings& runtimeSettings)

        : SettingObserver({Setting::MdnsName}),
          _jobController(jobController),
          _motionState(motionState),
          _fileManager(fileManager),
          _wifiController(wifiController),
          _settingPersistence(settingsPersistence),
          _runtimeSettings(runtimeSettings),
          _server(80)
    {}
    
    ~WebInterface();

    void init();
    void update();

private:
    JobController& _jobController;
    MotionState& _motionState;
    FileManager& _fileManager;
    SettingPersistence& _settingPersistence;
    RuntimeSettings& _runtimeSettings;
    WifiController& _wifiController;

    WebServer _server; // HTTP server (port 80)
    bool _serverStarted = false;

    void setupServer();

    // HTTP Handlers
    void handleFileList();
    void handleStartJob();
    void handleAbortJob();
    void handlePauseJob();
    void handleResumeJob();
    void handleUpload();
    void handleGetSettings();
    void handleSetSetting();

    void onRelevantSettingsChanged() override;
};