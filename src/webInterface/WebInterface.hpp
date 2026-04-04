#pragma once

#include <WebServer.h>

#include "systemServices/MotionState.hpp"
#include "storage/FileManager.hpp"
#include "jobController/JobController.hpp"
#include "settings/SettingsObserver.hpp"

// Forward declarations
class SettingsRepository;

class WebInterface : public SettingsObserver
{
public:
    WebInterface(JobController& jobController, MotionState& motionState, FileManager& fileManager, SettingsRepository& settingsRepository);
    
    ~WebInterface() = default;

    // Main lifecycle
    void init();      // Start WiFi connection (non-blocking)
    void update();    // Poll WiFi status, handle HTTP requests
    bool isWiFiConnected() const;

private:
    // Dependencies (references, never null)
    JobController& _jobController;
    MotionState& _motionState;
    FileManager& _fileManager;
    SettingsRepository& _settingsRepository;

    // WiFi/Server state
    WebServer _server;           // HTTP server (port 80)
    bool _serverStarted = false;

    // ====== WiFi Setup (Core 0) ======
    void startWiFiConnection();
    void setupServer();

    // ====== HTTP Handlers ======
    void handleFileList();
    void handleStartJob();
    void handleAbortJob();
    void handlePauseJob();
    void handleResumeJob();
    void handleUpload();

    // ====== SettingsObserver Implementation ======
    void onNetworkSettingsChanged(const NetworkSettings& newSettings) override;
};