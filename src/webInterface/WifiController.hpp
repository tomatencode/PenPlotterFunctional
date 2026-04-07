#pragma once

#include "settings/SettingObserver.hpp"
#include "settings/RuntimeSettings.hpp"
#include "settings/SettingPersistence.hpp"

class WifiController : public SettingObserver
{
public:
    explicit WifiController(SettingPersistence& settingPersistence, RuntimeSettings& runtimeSettings)
        : SettingObserver({Setting::SSID, Setting::Password}),
          _runtimeSettings(runtimeSettings),
          _settingPersistence(settingPersistence)
    {}

    ~WifiController();

    void init();
    void update();
    bool isConnected() const;

private:
    RuntimeSettings& _runtimeSettings;
    SettingPersistence& _settingPersistence;

    bool _wifiConfigured = false;
    uint32_t _lastConnectionAttemptMs = 0;
    uint32_t _reconnectIntervalMs = 5000;
    uint32_t _configureTimeoutMs = 100;
    uint16_t _maxConnectionAttempts = 10;
    uint16_t _currentConnectionAttempts = 0;

    void configureWifi();
    void attemptConnection();
    void onRelevantSettingsChanged() override;
};
