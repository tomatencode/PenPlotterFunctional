#pragma once
#include "SettingObserver.hpp"
#include "RuntimeSettings.hpp"
#include "Settings.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include <memory>

class SettingPercistence {
public:
    explicit SettingPercistence(RuntimeSettings& runtimeSettings)
        : _runtimeSettings(runtimeSettings)
    {}

    // Initialize: load all settings from NVS at startup
    void init();

    // setters: persist → RuntimeSettings → notify
    void setSSID(const std::string& value);
    void setPassword(const std::string& value);
    void setMdnsName(const std::string& value);
    void setDriverCurrent_mA(float value);
    void setMicrosteps(float value);
    void setDrawFeedRate_mm_per_s(float value);
    void setTravelFeedRate_mm_per_s(float value);
    void setHomingSpeed_stp_per_s(float value);
    void setStallguardThreshold(float value);
    void setPenUpAngle_deg(float value);
    void setPenDownAngle_deg(float value);

    void clearAllSettings();  // Clear all NVS data and reload defaults

    // ====== Observer Management ======
    void addObserver(SettingObserver* observer);
    void removeObserver(SettingObserver* observer);

private:
    // Notify all observers interested in a specific setting
    void notifyObservers(Setting setting) const;

    std::vector<SettingObserver*> _observers;

    void loadSettings();

    // Reference to cross-core atomic Setting Storage
    RuntimeSettings& _runtimeSettings;
};