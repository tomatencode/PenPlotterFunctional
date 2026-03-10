#include "WebInterface.hpp"

#include <WiFi.h>
#include <ESPmDNS.h>
#include <FS.h>

// WiFi credentials
static const char* SSID = "Thomas_Wifi";
static const char* PASSWORD = "TotallyHarmless";

WebInterface::WebInterface() : server(80) {}

// Non-blocking
void WebInterface::startWiFiConnection()
{
    WiFi.mode(WIFI_STA);
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    WiFi.begin(SSID, PASSWORD);
    _wifiStartTime = millis();
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
    if (_serverStarted) return; // Already set up
    if (WiFi.status() != WL_CONNECTED) return; // Should not happen, but guard just in case

    Serial.println();
    Serial.print("Connected! IP address: ");
    Serial.println(WiFi.localIP());

    // mDNS 
    if (!MDNS.begin("esp32"))
    {
        Serial.println("Error starting mDNS");
    }
    else
    {
        Serial.println("mDNS started: http://esp32.local");
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