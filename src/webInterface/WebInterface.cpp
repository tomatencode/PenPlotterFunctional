#include "WebInterface.hpp"
#include "settings/SettingsRepository.hpp"

#include <WiFi.h>
#include <ESPmDNS.h>

// ============================================================================
// Constructor
// ============================================================================

WebInterface::WebInterface(JobController& jobController, MotionState& motionState, FileManager& fileManager, SettingsRepository& settingsRepository)
    : _jobController(jobController), _motionState(motionState), _fileManager(fileManager),
      _settingsRepository(settingsRepository), _server(80)
{
}

// ============================================================================
// Lifecycle: init() and update()
// ============================================================================

void WebInterface::init() {
    Serial.println("WebInterface: Starting WiFi connection...");
    startWiFiConnection();
}

void WebInterface::update() {
    // Poll WiFi connection status in background
    if (!_serverStarted && WiFi.status() == WL_CONNECTED) {
        setupServer();
    }

    // Handle incoming HTTP requests
    if (_serverStarted) {
        _server.handleClient();
    }
}

bool WebInterface::isWiFiConnected() const {
    return WiFi.status() == WL_CONNECTED;
}

// ============================================================================
// WiFi Connection Setup (Core 0 - non-blocking)
// ============================================================================

void WebInterface::startWiFiConnection() {
    const NetworkSettings& netSettings = _settingsRepository.getNetworkSettings();

    Serial.printf("WebInterface: Configuring WiFi with SSID '%s'\n", netSettings.ssid.c_str());

    WiFi.mode(WIFI_STA);
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    WiFi.begin(netSettings.ssid.c_str(), netSettings.password.c_str());
    Serial.printf("WebInterface: Connecting to WiFi '%s'...\n", netSettings.ssid.c_str());
}

void WebInterface::setupServer() {
    if (_serverStarted || WiFi.status() != WL_CONNECTED) {
        return;  // Already started or WiFi not connected
    }

    Serial.print("WebInterface: WiFi connected! IP: ");
    Serial.println(WiFi.localIP());

    const NetworkSettings& netSettings = _settingsRepository.getNetworkSettings();

    // Start mDNS
    if (!MDNS.begin(netSettings.mdnsName.c_str())) {
        Serial.println("WebInterface: ERROR - Failed to start mDNS");
    } else {
        Serial.printf("WebInterface: mDNS started - http://%s.local\n", netSettings.mdnsName.c_str());
    }

    // Register HTTP route handlers
    _server.on("/files", HTTP_GET, [this]() { handleFileList(); });
    _server.on("/start", HTTP_POST, [this]() { handleStartJob(); });
    _server.on("/abort", HTTP_POST, [this]() { handleAbortJob(); });
    _server.on("/pause", HTTP_POST, [this]() { handlePauseJob(); });
    _server.on("/resume", HTTP_POST, [this]() { handleResumeJob(); });
    _server.on("/upload", HTTP_POST, [this]() {}, [this]() { handleUpload(); });

    _server.begin();
    _serverStarted = true;

    Serial.println("WebInterface: HTTP server started on port 80");
}

// ============================================================================
// SettingsObserver: React to Network Settings Changes
// ============================================================================

void WebInterface::onNetworkSettingsChanged(const NetworkSettings& newSettings) {
    Serial.println("WebInterface: Network settings changed, reconnecting WiFi...");
    
    // Disconnect existing WiFi
    WiFi.disconnect(true);
    _serverStarted = false;
    
    // Brief delay to ensure WiFi is fully shut down
    vTaskDelay(500 / portTICK_PERIOD_MS);
    
    // Reconnect with new credentials
    WiFi.begin(newSettings.ssid.c_str(), newSettings.password.c_str());
    Serial.printf("WebInterface: Reconnecting with SSID '%s'\n", newSettings.ssid.c_str());
}