#include "WebInterface.hpp"

#include <WiFi.h>
#include <ESPmDNS.h>

void WebInterface::init() {
    Serial.println("WebInterface initializing...");
    _settingPersistence.registerObserver(this);
}

WebInterface::~WebInterface() {
    _settingPersistence.unregisterObserver(this);
}

void WebInterface::update() {
    SettingObserver::checkIfSettingsChanged();

    if (!_wifiController.isConnected()) {
        return; // Don't attempt to start server if WiFi isn't connected
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

    // mDNS server
    if (!MDNS.begin(_runtimeSettings.getMdnsName().c_str())) {
        Serial.println("Error starting mDNS");
    }
    else {
        Serial.print("mDNS started: http://");
        Serial.print(_runtimeSettings.getMdnsName().c_str());
        Serial.println(".local");
    }

    // HTTP server routes
    _server.on("/files", HTTP_GET, [this]() { handleFileList(); });
    _server.on("/start", HTTP_POST, [this]() { handleStartJob(); });
    _server.on("/abort", HTTP_POST, [this]() { handleAbortJob(); });
    _server.on("/upload", HTTP_POST, [this]() {}, [this]() { handleUpload(); });
    _server.on("/pause", HTTP_POST, [this]() { handlePauseJob(); });
    _server.on("/resume", HTTP_POST, [this]() { handleResumeJob(); });
    _server.on("/settings", HTTP_GET, [this]() { handleGetSettings(); });
    _server.on("/settings", HTTP_POST, [this]() { handleSetSetting(); });


    _server.begin();
    _serverStarted = true;

    Serial.println("HTTP server started");
}

void WebInterface::onRelevantSettingsChanged() {
    // mDNS name has changed, restart mDNS with new name
    Serial.println("mDNS name changed, restarting mDNS...");
    MDNS.end();
    if (!MDNS.begin(_runtimeSettings.getMdnsName().c_str())) {
        Serial.println("Error restarting mDNS");
    }
    else {
        Serial.print("mDNS restarted: http://");
        Serial.print(_runtimeSettings.getMdnsName().c_str());
        Serial.println(".local");
    }
}