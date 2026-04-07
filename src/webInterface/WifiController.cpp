#include "WifiController.hpp"

#include <WiFi.h>

void WifiController::init() {
    Serial.println("WifiController initializing (WiFi connects in background)...");
    _settingPersistence.registerObserver(this);
    configureWifi();
}

WifiController::~WifiController() {
    _settingPersistence.unregisterObserver(this);
}

void WifiController::update() {
    SettingObserver::checkIfSettingsChanged();

    if (!isConnected()) {
        attemptConnection();
        return;
    }

    if (_currentConnectionAttempts > 0) {
        Serial.println("WiFi connected successfully!");
        _currentConnectionAttempts = 0;
    }
}

bool WifiController::isConnected() const {
    return WiFi.status() == WL_CONNECTED;
}
    
void WifiController::configureWifi() {
    WiFi.mode(WIFI_STA);

    _wifiConfigured = true;
    _lastConnectionAttemptMs = millis() - _reconnectIntervalMs + _configureTimeoutMs;
    _currentConnectionAttempts = 0;
}

void WifiController::attemptConnection() {
    if (!_wifiConfigured) return;
    if (isConnected()) return;
    if (_currentConnectionAttempts >= _maxConnectionAttempts) return;
    if (millis() - _lastConnectionAttemptMs < _reconnectIntervalMs) return;

    std::string ssid = _runtimeSettings.getSSID();
    std::string password = _runtimeSettings.getPassword();

    if (ssid.empty()) return;

    Serial.println("Attempting WiFi connection...");
    WiFi.begin(ssid.c_str(), password.c_str());

    _currentConnectionAttempts++;
    _lastConnectionAttemptMs = millis();

    if (_currentConnectionAttempts == _maxConnectionAttempts) {
        Serial.println("Max WiFi connection attempts reached.");
    }
}

void WifiController::onRelevantSettingsChanged() {
    // WiFi credentials have changed, reconnect with new credentials
    Serial.println("Network settings changed, reconnecting WiFi...");

    WiFi.disconnect(true);  // Disconnect and turn off WiFi

    // Small delay to ensure WiFi is fully shut down
    vTaskDelay(500 / portTICK_PERIOD_MS);

    configureWifi();
}
