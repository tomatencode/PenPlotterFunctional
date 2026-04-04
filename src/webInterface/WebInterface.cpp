#include "WebInterface.hpp"

#include <WiFi.h>
#include <ESPmDNS.h>

void WebInterface::init() {
    _settingsRepository.addObserver(this);

    Serial.println("WebInterface initializing (WiFi connects in background)...");
    startWiFiConnection();
}

bool WebInterface::isWiFiConnected() const {
    return WiFi.status() == WL_CONNECTED;
}


void WebInterface::update() {
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
    const NetworkSettings& netSettings = _settingsRepository.getNetworkSettings();

    WiFi.mode(WIFI_STA);
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    WiFi.begin(netSettings.ssid.c_str(), netSettings.password.c_str());

    // Wait for connection
    vTaskDelay(500 / portTICK_PERIOD_MS);

    Serial.print("WiFi Connected! IP address: ");
    Serial.println(WiFi.localIP());
}

void WebInterface::setupServer() {
    if (_serverStarted) return;
    if (WiFi.status() != WL_CONNECTED) return;

    const NetworkSettings& netSettings = _settingsRepository.getNetworkSettings();

    // mDNS server
    if (!MDNS.begin(netSettings.mdnsName.c_str())) {
        Serial.println("Error starting mDNS");
    }
    else {
        Serial.print("mDNS started: http://");
        Serial.print(netSettings.mdnsName.c_str());
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

void WebInterface::onNetworkSettingsChanged(const NetworkSettings& newSettings) {
    // WiFi credentials have changed, reconnect with new credentials
    Serial.println("Network settings changed, reconnecting WiFi...");
    
    WiFi.disconnect(true);  // Disconnect and turn off WiFi
    _serverStarted = false;
    
    // Small delay to ensure WiFi is fully shut down
    vTaskDelay(500 / portTICK_PERIOD_MS);
    
    // Restart WiFi connection with new credentials
    WiFi.begin(newSettings.ssid.c_str(), newSettings.password.c_str());
}