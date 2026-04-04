#include "SettingsRepository.hpp"
#include "defaults/PlottingDefaults.hpp"
#include "defaults/WifiDefaults.hpp"
#include <Preferences.h>
#include <algorithm>

// ============================================================================
// Constructor and Initialization
// ============================================================================

SettingsRepository::SettingsRepository(RuntimeSettings& runtimeSettings)
    : _runtimeSettings(runtimeSettings) {
    // Members initialized to zero/empty via default constructors
}

void SettingsRepository::init() {
    // Load all settings from NVS on startup
    // This happens once, in Tasks.cpp, before ApplicationManager::init() is called
    loadNetworkSettings();
    loadPlottingSettings();
    loadPenSettings();

    // Populate RuntimeSettings with the loaded values
    // (the lock-free atomic bridge for Core 1 to read motion settings)
    _runtimeSettings.setDriverCurrent_mA(_plotting.driverCurrent_mA);
    _runtimeSettings.setMicrosteps(_plotting.microsteps);
    _runtimeSettings.setDrawFeedRate_mm_per_s(_plotting.drawFeedRate_mm_per_s);
    _runtimeSettings.setTravelFeedRate_mm_per_s(_plotting.travelFeedRate_mm_per_s);
    _runtimeSettings.setHomingSpeed_stp_per_s(_plotting.homingSpeed_stp_per_s);
    _runtimeSettings.setStallguardThreshold(_plotting.stallguardThreshold);
    _runtimeSettings.setPenUpAngle_deg(_pen.upAngle_deg);
    _runtimeSettings.setPenDownAngle_deg(_pen.downAngle_deg);
}

// ============================================================================
// Getters (return current in-RAM copies)
// ============================================================================

NetworkSettings SettingsRepository::getNetworkSettings() const {
    return _network;
}

PlottingSettings SettingsRepository::getPlottingSettings() const {
    return _plotting;
}

PenSettings SettingsRepository::getPenSettings() const {
    return _pen;
}

// ============================================================================
// Setters (persist → RAM → RuntimeSettings → notify)
// ============================================================================

void SettingsRepository::updateNetworkSettings(const NetworkSettings& settings) {
    // TODO: Validate settings (e.g., SSID length, password strength?)
    
    _network = settings;
    persistNetworkSettings();
    notifyNetworkSettingsChanged();
    
    // Note: WebInterface observes this event and handles WiFi reconnection
}

void SettingsRepository::updatePlottingSettings(const PlottingSettings& settings) {
    // TODO: Validate (e.g., feed rates > 0?)
    
    _plotting = settings;
    persistPlottingSettings();

    // Update the atomic bridge immediately so Core 1 sees the new values
    // on the next G-code command execution
    _runtimeSettings.setDriverCurrent_mA(_plotting.driverCurrent_mA);
    _runtimeSettings.setMicrosteps(_plotting.microsteps);
    _runtimeSettings.setDrawFeedRate_mm_per_s(_plotting.drawFeedRate_mm_per_s);
    _runtimeSettings.setTravelFeedRate_mm_per_s(_plotting.travelFeedRate_mm_per_s);
    _runtimeSettings.setHomingSpeed_stp_per_s(_plotting.homingSpeed_stp_per_s);
    _runtimeSettings.setStallguardThreshold(_plotting.stallguardThreshold);

    notifyPlottingSettingsChanged();
}

void SettingsRepository::updatePenSettings(const PenSettings& settings) {
    // TODO: Validate (e.g., angles in range 0–180?)
    
    _pen = settings;
    persistPenSettings();

    // Update the atomic bridge
    _runtimeSettings.setPenUpAngle_deg(_pen.upAngle_deg);
    _runtimeSettings.setPenDownAngle_deg(_pen.downAngle_deg);

    notifyPenSettingsChanged();
}

// ============================================================================
// Observer Management
// ============================================================================

void SettingsRepository::addObserver(SettingsObserver* observer) {
    if (observer) {
        _observers.push_back(observer);
    }
}

void SettingsRepository::removeObserver(SettingsObserver* observer) {
    auto it = std::find(_observers.begin(), _observers.end(), observer);
    if (it != _observers.end()) {
        _observers.erase(it);
    }
}

void SettingsRepository::notifyNetworkSettingsChanged() const {
    for (auto observer : _observers) {
        observer->onNetworkSettingsChanged(_network);
    }
}

void SettingsRepository::notifyPlottingSettingsChanged() const {
    for (auto observer : _observers) {
        observer->onPlottingSettingsChanged(_plotting);
    }
}

void SettingsRepository::notifyPenSettingsChanged() const {
    for (auto observer : _observers) {
        observer->onPenSettingsChanged(_pen);
    }
}

// ============================================================================
// NVS Persistence (ESP32 Non-Volatile Storage / flash)
// ============================================================================
//
// Each settings group gets its own Preferences namespace.
// The Preferences API is simple:
//
//   Preferences prefs;
//   prefs.begin("namespace", read_only);
//   prefs.putFloat("key", value);
//   prefs.getFloat("key", default_value);
//   prefs.end();
//
// Notes:
// - NVS operations are slow (flash I/O), but only happen when user saves settings
// - Motion loop never does NVS operations
// - A write also validates the default value if the key doesn't exist
//

void SettingsRepository::persistNetworkSettings() {
    Preferences prefs;
    prefs.begin("network", false);  // false = read-write mode
    
    prefs.putString("ssid", _network.ssid.c_str());
    prefs.putString("password", _network.password.c_str());
    prefs.putString("mdnsName", _network.mdnsName.c_str());
    
    prefs.end();
}

void SettingsRepository::persistPlottingSettings() {
    Preferences prefs;
    prefs.begin("machine", false);

    prefs.putFloat("driverCurrent", _plotting.driverCurrent_mA);
    prefs.putFloat("microsteps", _plotting.microsteps);
    prefs.putFloat("drawFeed", _plotting.drawFeedRate_mm_per_s);
    prefs.putFloat("travelFeed", _plotting.travelFeedRate_mm_per_s);
    prefs.putFloat("homingSpeed", _plotting.homingSpeed_stp_per_s);
    prefs.putFloat("stallguardThreshold", _plotting.stallguardThreshold);

    prefs.end();
}

void SettingsRepository::persistPenSettings() {
    Preferences prefs;
    prefs.begin("pen", false);

    prefs.putFloat("upAngle", _pen.upAngle_deg);
    prefs.putFloat("downAngle", _pen.downAngle_deg);

    prefs.end();
}

void SettingsRepository::loadNetworkSettings() {
    Preferences prefs;
    prefs.begin("network", true);  // true = read-only mode

    // Use hardcoded config header values as defaults on first boot
    // Construct std::string from c_str() (data is copied into std::string)
    _network.ssid = std::string(prefs.getString("ssid", SSID).c_str());
    _network.password = std::string(prefs.getString("password", PASSWORD).c_str());
    _network.mdnsName = std::string(prefs.getString("mdnsName", MDNS_NAME).c_str());

    prefs.end();
}

void SettingsRepository::loadPlottingSettings() {
    Preferences prefs;
    prefs.begin("machine", true);
    
    _plotting.driverCurrent_mA = prefs.getFloat("driverCurrent", DRIVER_CURRENT_MA);
    _plotting.microsteps = prefs.getFloat("microsteps", MICROSTEPS);
    _plotting.drawFeedRate_mm_per_s = prefs.getFloat("drawFeed", FEED_RATE_DRAW_MM_PER_S);
    _plotting.travelFeedRate_mm_per_s = prefs.getFloat("travelFeed", FEED_RATE_TRAVEL_MM_PER_S);
    _plotting.homingSpeed_stp_per_s = prefs.getFloat("homingSpeed", HOMING_SPEED_STP_PER_S);
    _plotting.stallguardThreshold = prefs.getFloat("stallguardThreshold", HOMING_STALLGUARD_THRESHOLD);

    prefs.end();
}

void SettingsRepository::loadPenSettings() {
    Preferences prefs;
    prefs.begin("pen", true);

    _pen.upAngle_deg = prefs.getFloat("upAngle", PEN_UP_DEG);
    _pen.downAngle_deg = prefs.getFloat("downAngle", PEN_DOWN_DEG);

    prefs.end();
}
