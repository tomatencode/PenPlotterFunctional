#include "WebInterface.hpp"
#include "config/wifi_config.hpp"

#include <WiFi.h>
#include <ESPmDNS.h>

// Non-blocking
void WebInterface::startWiFiConnection()
{
    WiFi.mode(WIFI_STA);
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    WiFi.begin(SSID, PASSWORD);
    _wifiInitialized = true;
}

void WebInterface::checkWiFiStatus()
{
    if (!_wifiInitialized) return; // Should not happen, but guard just in case

    wl_status_t status = WiFi.status();

    if (status == WL_CONNECTED && !_serverStarted)
    {
        setupServer();
        _serverStarted = true;
    }
}

void WebInterface::setupServer()
{
    if (_serverStarted) return;
    if (WiFi.status() != WL_CONNECTED) return;

    Serial.print("WiFi Connected! IP address: ");
    Serial.println(WiFi.localIP());

    // mDNS 
    if (!MDNS.begin(MDNS_NAME))
    {
        Serial.println("Error starting mDNS");
    }
    else
    {
        Serial.print("mDNS started: http://");
        Serial.print(MDNS_NAME);
        Serial.println(".local");
    }

    // HTTP server routes
    server.on("/files", HTTP_GET, [this]() { handleFileList(); });
    server.on("/start", HTTP_POST, [this]() { handleStartJob(); });
    server.on("/abort", HTTP_POST, [this]() { handleAbortJob(); });
    server.on("/upload", HTTP_POST, [this]() {}, [this]() { handleUpload(); });
    server.on("/pause", HTTP_POST, [this]() { handlePauseJob(); });
    server.on("/resume", HTTP_POST, [this]() { handleResumeJob(); });

    server.begin();
}

void WebInterface::init()
{
    Serial.println("WebInterface initializing (WiFi connects in background)...");
    startWiFiConnection();
}

bool WebInterface::isWiFiConnected() const
{
    return WiFi.status() == WL_CONNECTED;
}

void WebInterface::update()
{
    if (!_serverStarted)
    {
        checkWiFiStatus();
    }

    if (_serverStarted)
    {
        server.handleClient();
    }
}