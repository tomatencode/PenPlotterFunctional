#include "WifiController.hpp"

#include <WiFi.h>
#include <esp_log.h>

static const char* TAG = "WifiController";

void WifiController::init() {
    ESP_LOGI(TAG, "Initializing (WiFi connects in background)");
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
        ESP_LOGI(TAG, "WiFi connected to SSID: %s, IP: %s",
                 WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
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

    ESP_LOGI(TAG, "Attempting WiFi connection");
    WiFi.begin(ssid.c_str(), password.c_str());

    _currentConnectionAttempts++;
    _lastConnectionAttemptMs = millis();

    if (_currentConnectionAttempts == _maxConnectionAttempts) {
        ESP_LOGW(TAG, "Max WiFi connection attempts reached");
    }
}

void WifiController::onRelevantSettingsChanged() {
    // WiFi credentials have changed, reconnect with new credentials
    ESP_LOGI(TAG, "Network settings changed, reconnecting WiFi");

    WiFi.disconnect(true);  // Disconnect and turn off WiFi

    // Reset connection attempt tracking so it will try to connect immediately with new settings
    _lastConnectionAttemptMs = millis() - _reconnectIntervalMs + _configureTimeoutMs;
    _currentConnectionAttempts = 0;
}
