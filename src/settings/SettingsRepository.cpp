#include "SettingsRepository.hpp"
#include "../systemServices/RuntimeSettings.hpp"
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
    _runtimeSettings.setDrawFeedRate(_plotting.drawFeedRate);
    _runtimeSettings.setTravelFeedRate(_plotting.travelFeedRate);
    _runtimeSettings.setHomingSpeed(_plotting.homingSpeed);
    _runtimeSettings.setStallguardThreshold(_plotting.stallguardThreshold);
    _runtimeSettings.setPenUpAngle(_pen.upAngle);
    _runtimeSettings.setPenDownAngle(_pen.downAngle);
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
    _runtimeSettings.setDrawFeedRate(_plotting.drawFeedRate);
    _runtimeSettings.setTravelFeedRate(_plotting.travelFeedRate);
    _runtimeSettings.setHomingSpeed(_plotting.homingSpeed);
    _runtimeSettings.setStallguardThreshold(_plotting.stallguardThreshold);

    notifyPlottingSettingsChanged();
}

void SettingsRepository::updatePenSettings(const PenSettings& settings) {
    // TODO: Validate (e.g., angles in range 0–180?)
    
    _pen = settings;
    persistPenSettings();

    // Update the atomic bridge
    _runtimeSettings.setPenUpAngle(_pen.upAngle);
    _runtimeSettings.setPenDownAngle(_pen.downAngle);

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
    prefs.putFloat("drawFeed", _plotting.drawFeedRate);
    prefs.putFloat("travelFeed", _plotting.travelFeedRate);
    prefs.putFloat("homingSpeed", _plotting.homingSpeed);
    prefs.putFloat("stallguardThreshold", _plotting.stallguardThreshold);

    prefs.end();
}

void SettingsRepository::persistPenSettings() {
    Preferences prefs;
    prefs.begin("pen", false);

    prefs.putFloat("upAngle", _pen.upAngle);
    prefs.putFloat("downAngle", _pen.downAngle);

    prefs.end();
}

void SettingsRepository::loadNetworkSettings() {
    Preferences prefs;
    prefs.begin("network", true);  // true = read-only mode

    // Use hardcoded config header values as defaults on first boot
    String ssid = prefs.getString("ssid", SSID);
    _network.ssid = ssid.c_str();
    String password = prefs.getString("password", PASSWORD);
    _network.password = password.c_str();
    String mdnsName = prefs.getString("mdnsName", MDNS_NAME);
    _network.mdnsName = mdnsName.c_str();

    prefs.end();
}

void SettingsRepository::loadPlottingSettings() {
    Preferences prefs;
    prefs.begin("machine", true);
    
    _plotting.driverCurrent_mA = prefs.getFloat("driverCurrent", DRIVER_CURRENT_MA);
    _plotting.microsteps = prefs.getFloat("microsteps", MICROSTEPS);
    _plotting.drawFeedRate = prefs.getFloat("drawFeed", FEED_RATE_DRAW_MM_PER_S);
    _plotting.travelFeedRate = prefs.getFloat("travelFeed", FEED_RATE_TRAVEL_MM_PER_S);
    _plotting.homingSpeed = prefs.getFloat("homingSpeed", HOMING_SPEED_STPS_PER_S);
    _plotting.stallguardThreshold = prefs.getFloat("stallguardThreshold", HOMING_STALLGUARD_THRESHOLD);

    prefs.end();
}

void SettingsRepository::loadPenSettings() {
    Preferences prefs;
    prefs.begin("pen", true);

    _pen.upAngle = prefs.getFloat("upAngle", PEN_UP_ANGLE);
    _pen.downAngle = prefs.getFloat("downAngle", PEN_DOWN_ANGLE);

    prefs.end();
}
