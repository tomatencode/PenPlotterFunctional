#pragma once

#include <WebServer.h>

#include "rtos/MotionState.hpp"
#include "storage/FileManager.hpp"
#include "gcode/GCodeSender.hpp"
#include "jobController/JobController.hpp"
#include "settings/SettingPersistence.hpp"
#include "settings/RuntimeSettings.hpp"
#include "settings/SettingObserver.hpp"
#include "WifiController.hpp"

class WebInterface : public SettingObserver
{
public:
    WebInterface(JobController& jobController,
                 GCodeSender& gcodeSender,
                 MotionState& motionState,
                 FileManager& fileManager,
                 WifiController& wifiController,
                 SettingPersistence& settingsPersistence,
                 RuntimeSettings& runtimeSettings)

        : SettingObserver({Setting::MdnsName}),
          _jobController(jobController),
          _gcodeSender(gcodeSender),
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
    GCodeSender& _gcodeSender;
    MotionState& _motionState;
    FileManager& _fileManager;
    SettingPersistence& _settingPersistence;
    RuntimeSettings& _runtimeSettings;
    WifiController& _wifiController;

    WebServer _server; // HTTP server (port 80)
    bool _serverStarted = false;

    // Upload state tracking
    File _currentUploadFile;
    std::string _currentUploadPath;
    std::string _currentTempPath;
    size_t _uploadedBytes = 0;
    static constexpr size_t MAX_UPLOAD_SIZE = 10 * 1024 * 1024; // 10 MB limit

    void setupServer();

    // Upload helpers
    bool validateFileName(const std::string& filename);
    void resetUploadState();

    // HTTP handlers
    void handleUpload();
    void handleListJobs();
    void handleStartJob();
    void handleAbortJob();
    void handlePauseJob();
    void handleResumeJob();

    void handleGetJobStatus();
    void handlePlotActive();
    void handleGetPlotPaused();
    void handleGetCurrentPlot();
    void handleGetPlotProgress();
    void handleGetCurrentLine();
    void handleGetTotalLines();

    void handleExecuteLine();

    void handleGetSetting();
    void handleSetSetting();
    void handleGetAllSettings();

    void onRelevantSettingsChanged() override;
};