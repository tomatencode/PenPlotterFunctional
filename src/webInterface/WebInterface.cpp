#include "WebInterface.hpp"

#include <WiFi.h>
#include <ESPmDNS.h>
#include <esp_log.h>

static const char* TAG = "WebInterface";

void WebInterface::init() {
    ESP_LOGI(TAG, "Initializing");
    _settingPersistence.registerObserver(this);

    _wsServer.onEvent([this](uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
        handleWebSocketEvent(num, type, payload, length);
    });

    // Register routes once, they persist across server restarts
    _server.on("/iteration", HTTP_GET, [this]() { handleGetItteration(); });
    _server.on("/firmwareVersion", HTTP_GET, [this]() { handleGetFirmwareVersion(); });
    _server.on("/workspace", HTTP_GET, [this]() { handleGetWorkspace(); });
    
    _server.on("/position", HTTP_GET, [this]() { handleGetPosition(); });
    _server.on("/motionState", HTTP_GET, [this]() { handleGetMotionState(); });
    _server.on("/activePenSlot", HTTP_GET, [this]() { handleGetActivePenSlot(); });

    _server.on("/name", HTTP_GET, [this]() { handleGetName(); });
    _server.on("/name", HTTP_PUT, [this]() { changeName(); });
    _server.on("/mdnsName", HTTP_GET, [this]() { handleGetMDNSName(); });
    _server.on("/mdnsName", HTTP_PUT, [this]() { changeMDNSName(); });
    
    _server.on("/upload", HTTP_POST, [this]() {}, [this]() { handleUploadJob(); });
    _server.on("/plotFiles", HTTP_DELETE, [this]() { handleDeleteJob(); });
    _server.on("/plotFiles", HTTP_GET,  [this]() { handleListJobs(); });
    _server.on("/start", HTTP_POST, [this]() { handleStartJob(); });
    _server.on("/abort", HTTP_POST, [this]() { handleAbortJob(); });
    _server.on("/pause", HTTP_POST, [this]() { handlePauseJob(); });
    _server.on("/resume", HTTP_POST, [this]() { handleResumeJob(); });

    _server.on("/jobStatus", HTTP_GET, [this]() { handleGetJobStatus(); });

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
            _wsServer.close();
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
        _wsServer.loop();

        unsigned long now = millis();
        if (now - _lastStateMs >= 100) {
            _lastStateMs = now;
            broadcastState();
        }
    }
}

void WebInterface::setupServer() {
    if (_serverStarted) return;
    if (!_wifiController.isConnected()) return;

    if (!(MDNS.begin(_runtimeSettings.mdnsName().c_str()) && MDNS.addService("http", "tcp", 80))) {
        ESP_LOGE(TAG, "Error starting mDNS");
    }
    else {
        MDNS.addServiceTxt("http", "tcp", "device", "pnplttr");
        ESP_LOGI(TAG, "mDNS started: http://%s.local", _runtimeSettings.mdnsName().c_str());
    }

    _server.begin();
    _wsServer.begin();
    _serverStarted = true;

    ESP_LOGI(TAG, "HTTP and WebSocket server started");
}

void WebInterface::onRelevantSettingsChanged() {
    // mDNS name has changed, restart mDNS with new name
    ESP_LOGI(TAG, "mDNS name changed, restarting mDNS");
    MDNS.end();
    if (!MDNS.begin(_runtimeSettings.mdnsName().c_str())) {
        ESP_LOGE(TAG, "Error restarting mDNS");
    }
    else {
        ESP_LOGI(TAG, "mDNS restarted: http://%s.local", _runtimeSettings.mdnsName().c_str());
    }
}