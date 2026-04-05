#include "WebInterface.hpp"

#include <WiFi.h>
#include <ESPmDNS.h>

void WebInterface::init() {
    Serial.println("WebInterface initializing (WiFi connects in background)...");
    startWiFiConnection();

    _settingPersistence.registerObserver(this);
}

WebInterface::~WebInterface() {
    _settingPersistence.unregisterObserver(this);
}

bool WebInterface::isWiFiConnected() const {
    return WiFi.status() == WL_CONNECTED;
}


void WebInterface::update() {
    SettingObserver::checkIfSettingsChanged();

    if (!_serverStarted && WiFi.status() == WL_CONNECTED) {
        setupServer();
    }

    if (_serverStarted)
    {
        _server.handleClient();
    }
}

// Non-blocking
void WebInterface::startWiFiConnection() {

    WiFi.mode(WIFI_STA);
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    WiFi.begin(_runtimeSettings.getSSID().c_str(), _runtimeSettings.getPassword().c_str());
}

void WebInterface::setupServer() {
    if (_serverStarted) return;
    if (WiFi.status() != WL_CONNECTED) return;


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

    _server.begin();
    _serverStarted = true;

    Serial.println("HTTP server started");
}

void WebInterface::onRelevantSettingsChanged() {
    // WiFi credentials have changed, reconnect with new credentials
    Serial.println("Network settings changed, reconnecting WiFi...");
    
    WiFi.disconnect(true);  // Disconnect and turn off WiFi
    _serverStarted = false;
    
    // Small delay to ensure WiFi is fully shut down
    vTaskDelay(500 / portTICK_PERIOD_MS);
    
    // Restart WiFi connection with new credentials
    WiFi.begin(_runtimeSettings.getSSID().c_str(), _runtimeSettings.getPassword().c_str());
}