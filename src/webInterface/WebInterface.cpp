#include "WebInterface.hpp"

#include <WiFi.h>
#include <ESPmDNS.h>
#include <esp_log.h>

static const char* TAG = "WebInterface";

void WebInterface::init() {
    ESP_LOGI(TAG, "Initializing");
    _settingPersistence.registerObserver(this);

    // Register routes once, they persist across server restarts
    _server.on("/upload", HTTP_POST, [this]() {}, [this]() { handleUploadJob(); });
    _server.on("/plotFiles", HTTP_DELETE, [this]() { handleDeleteJob(); });
    _server.on("/plotFiles", HTTP_GET,  [this]() { handleListJobs(); });
    _server.on("/start", HTTP_POST, [this]() { handleStartJob(); });
    _server.on("/abort", HTTP_POST, [this]() { handleAbortJob(); });
    _server.on("/pause", HTTP_POST, [this]() { handlePauseJob(); });
    _server.on("/resume", HTTP_POST, [this]() { handleResumeJob(); });

    _server.on("/jobStatus", HTTP_GET, [this]() { handleGetJobStatus(); });
    _server.on("/plotActive", HTTP_GET,  [this]() { handlePlotActive(); });
    _server.on("/plotPaused", HTTP_GET,  [this]() { handleGetPlotPaused(); });
    _server.on("/currentPlot", HTTP_GET,  [this]() { handleGetCurrentPlot(); });
    _server.on("/plotProgress", HTTP_GET,  [this]() { handleGetPlotProgress(); });
    _server.on("/plotCurrentLine", HTTP_GET,  [this]() { handleGetCurrentLine(); });
    _server.on("/plotTotalLines", HTTP_GET,  [this]() { handleGetTotalLines(); });

    _server.on("/execute", HTTP_POST, [this]() { handleExecuteLine(); });

    _server.on("/setting", HTTP_GET,  [this]() { handleGetSetting(); });
    _server.on("/setting", HTTP_PUT, [this]() { handleSetSetting(); });
    _server.on("/settings", HTTP_GET,  [this]() { handleGetAllSettings(); });
}

WebInterface::~WebInterface() {
    _settingPersistence.unregisterObserver(this);
}

void WebInterface::update() {
    SettingObserver::checkIfSettingsChanged();

    if (!_wifiController.isConnected()) {
        if (_serverStarted) {
            MDNS.end();
            _serverStarted = false;
            ESP_LOGI(TAG, "WiFi lost - HTTP server stopped");
        }
        return;
    }

    if (!_serverStarted) {
        setupServer();
    }

    if (_serverStarted)
    {
        _server.handleClient();
    }
}

void WebInterface::setupServer() {
    if (_serverStarted) return;
    if (!_wifiController.isConnected()) return;

    if (!MDNS.begin(_runtimeSettings.getMdnsName().c_str())) {
        ESP_LOGE(TAG, "Error starting mDNS");
    }
    else {
        ESP_LOGI(TAG, "mDNS started: http://%s.local", _runtimeSettings.getMdnsName().c_str());
    }

    _server.begin();
    _serverStarted = true;

    ESP_LOGI(TAG, "HTTP server started");
}

void WebInterface::onRelevantSettingsChanged() {
    // mDNS name has changed, restart mDNS with new name
    ESP_LOGI(TAG, "mDNS name changed, restarting mDNS");
    MDNS.end();
    if (!MDNS.begin(_runtimeSettings.getMdnsName().c_str())) {
        ESP_LOGE(TAG, "Error restarting mDNS");
    }
    else {
        ESP_LOGI(TAG, "mDNS restarted: http://%s.local", _runtimeSettings.getMdnsName().c_str());
    }
}