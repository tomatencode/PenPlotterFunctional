#include "SettingPersistence.hpp"
#include "defaults/PlottingDefaults.hpp"
#include "defaults/WifiDefaults.hpp"
#include <Preferences.h>
#include <algorithm>


void SettingPersistence::init() {
    loadSettings();
}


// Setters persist → RuntimeSettings → notify

void SettingPersistence::setSSID(const std::string& value) {
    Preferences prefs;
    prefs.begin("settings", false);
    prefs.putString("ssid", value.c_str());
    prefs.end();
    
    _runtimeSettings.setSSID(value);
    notifyObservers(Setting::SSID);
}

void SettingPersistence::setPassword(const std::string& value) {
    Preferences prefs;
    prefs.begin("settings", false);
    prefs.putString("password", value.c_str());
    prefs.end();
    
    _runtimeSettings.setPassword(value);
    notifyObservers(Setting::Password);
}

void SettingPersistence::setMdnsName(const std::string& value) {
    Preferences prefs;
    prefs.begin("settings", false);
    prefs.putString("mdnsName", value.c_str());
    prefs.end();
    
    _runtimeSettings.setMdnsName(value);
    notifyObservers(Setting::MdnsName);
}

void SettingPersistence::setDriverCurrent_mA(float value) {
    Preferences prefs;
    prefs.begin("settings", false);
    prefs.putFloat("driverCurrent", value);
    prefs.end();
    
    _runtimeSettings.setDriverCurrent_mA(value);
    notifyObservers(Setting::DriverCurrent);
}

void SettingPersistence::setMicrosteps(float value) {
    Preferences prefs;
    prefs.begin("settings", false);
    prefs.putFloat("microsteps", value);
    prefs.end();
    
    _runtimeSettings.setMicrosteps(value);
    notifyObservers(Setting::Microsteps);
}

void SettingPersistence::setDrawFeedRate_mm_per_s(float value) {
    Preferences prefs;
    prefs.begin("settings", false);
    prefs.putFloat("drawFeed", value);
    prefs.end();
    
    _runtimeSettings.setDrawFeedRate_mm_per_s(value);
    notifyObservers(Setting::DrawFeedRate);
}

void SettingPersistence::setTravelFeedRate_mm_per_s(float value) {
    Preferences prefs;
    prefs.begin("settings", false);
    prefs.putFloat("travelFeed", value);
    prefs.end();
    
    _runtimeSettings.setTravelFeedRate_mm_per_s(value);
    notifyObservers(Setting::TravelFeedRate);
}

void SettingPersistence::setHomingSpeed_stp_per_s(float value) {
    Preferences prefs;
    prefs.begin("settings", false);
    prefs.putFloat("homingSpeed", value);
    prefs.end();
    
    _runtimeSettings.setHomingSpeed_stp_per_s(value);
    notifyObservers(Setting::HomingSpeed);
}

void SettingPersistence::setHomingBackOffSpeed_stp_per_s(float value) {
    Preferences prefs;
    prefs.begin("settings", false);
    prefs.putFloat("homingBOspd", value);
    prefs.end();
    
    _runtimeSettings.setHomingBackOffSpeed_stp_per_s(value);
    notifyObservers(Setting::HomingBackOffSpeed);
}

void SettingPersistence::setStallguardThreshold(float value) {
    Preferences prefs;
    prefs.begin("settings", false);
    prefs.putFloat("SGThreshold", value);
    prefs.end();
    
    _runtimeSettings.setStallguardThreshold(value);
    notifyObservers(Setting::StallguardThreshold);
}

void SettingPersistence::setBackOffStepsX(uint16_t value) {
    Preferences prefs;
    prefs.begin("settings", false);
    prefs.putUShort("boStepsX", value);
    prefs.end();
    
    _runtimeSettings.setBackOffStepsX(value);
    notifyObservers(Setting::BackOffStepsX);
}

void SettingPersistence::setBackOffStepsY(uint16_t value) {
    Preferences prefs;
    prefs.begin("settings", false);
    prefs.putUShort("boStepsY", value);
    prefs.end();
    
    _runtimeSettings.setBackOffStepsY(value);
    notifyObservers(Setting::BackOffStepsY);
}

void SettingPersistence::setHomingTimeout_us(uint32_t value) {
    Preferences prefs;
    prefs.begin("settings", false);
    prefs.putUInt("homingTimeout", value);
    prefs.end();
    
    _runtimeSettings.setHomingTimeout_us(value);
    notifyObservers(Setting::HomingTimeout);
}

void SettingPersistence::setSGCheckInterval_ms(uint16_t value) {
    Preferences prefs;
    prefs.begin("settings", false);
    prefs.putUShort("SGCheckInt", value);
    prefs.end();
    
    _runtimeSettings.setSGCheckInterval_ms(value);
    notifyObservers(Setting::SGCheckInterval);
}

void SettingPersistence::setSGStartTimeout_ms(uint16_t value) {
    Preferences prefs;
    prefs.begin("settings", false);
    prefs.putUShort("SGStartTO", value);
    prefs.end();
    
    _runtimeSettings.setSGStartTimeout_ms(value);
    notifyObservers(Setting::SGStartTimeout);
}

void SettingPersistence::setSGHistorySize(uint8_t value) {
    Preferences prefs;
    prefs.begin("settings", false);
    prefs.putUChar("SGHistSize", value);
    prefs.end();
    
    _runtimeSettings.setSGHistorySize(value);
    notifyObservers(Setting::SGHistorySize);
}

void SettingPersistence::setPenUpAngle_deg(float value) {
    Preferences prefs;
    prefs.begin("settings", false);
    prefs.putFloat("upAngle", value);
    prefs.end();
    
    _runtimeSettings.setPenUpAngle_deg(value);
    notifyObservers(Setting::PenUpAngle);
}

void SettingPersistence::setPenDownAngle_deg(float value) {
    Preferences prefs;
    prefs.begin("settings", false);
    prefs.putFloat("downAngle", value);
    prefs.end();
    
    _runtimeSettings.setPenDownAngle_deg(value);
    notifyObservers(Setting::PenDownAngle);
}

void SettingPersistence::loadSettings() {
    Preferences prefs;
    prefs.begin("settings", true);  // true = read-only mode

    // Load each setting with a default fallback
    _runtimeSettings.setSSID(prefs.getString("ssid", SSID).c_str());
    _runtimeSettings.setPassword(prefs.getString("password", PASSWORD).c_str());
    _runtimeSettings.setMdnsName(prefs.getString("mdnsName", MDNS_NAME).c_str());
    _runtimeSettings.setDriverCurrent_mA(prefs.getFloat("driverCurrent", DRIVER_CURRENT_MA));
    _runtimeSettings.setMicrosteps(prefs.getFloat("microsteps", MICROSTEPS));
    _runtimeSettings.setDrawFeedRate_mm_per_s(prefs.getFloat("drawFeed", FEED_RATE_DRAW_MM_PER_S));
    _runtimeSettings.setTravelFeedRate_mm_per_s(prefs.getFloat("travelFeed", FEED_RATE_TRAVEL_MM_PER_S));
    _runtimeSettings.setHomingSpeed_stp_per_s(prefs.getFloat("homingSpeed", HOMING_SPEED_STP_PER_S));
    _runtimeSettings.setHomingBackOffSpeed_stp_per_s(prefs.getFloat("homingBOspd", HOMING_BACK_OFF_SPEED_STP_PER_S));
    _runtimeSettings.setStallguardThreshold(prefs.getFloat("SGThreshold", STALLGUARD_THRESHOLD));
    _runtimeSettings.setBackOffStepsX(prefs.getUShort("boStepsX", BACK_OFF_STEPS_X));
    _runtimeSettings.setBackOffStepsY(prefs.getUShort("boStepsY", BACK_OFF_STEPS_Y));
    _runtimeSettings.setHomingTimeout_us(prefs.getUInt("homingTimeout", HOMING_TIMEOUT_US));
    _runtimeSettings.setSGCheckInterval_ms(prefs.getUShort("SGCheckInt", SG_CHECK_INTERVAL_MS));
    _runtimeSettings.setSGStartTimeout_ms(prefs.getUShort("SGStartTO", SG_START_TIMEOUT_MS));
    _runtimeSettings.setSGHistorySize(prefs.getUChar("SGHistSize", SG_HISTORY_SIZE));
    _runtimeSettings.setPenUpAngle_deg(prefs.getFloat("upAngle", PEN_UP_DEG));
    _runtimeSettings.setPenDownAngle_deg(prefs.getFloat("downAngle", PEN_DOWN_DEG));
    
    prefs.end();
}

void SettingPersistence::clearAllSettings() {
    Preferences prefs;
    prefs.begin("settings", false);  // false = read-write mode
    prefs.clear();                   // Clear entire namespace
    prefs.end();
    
    Serial.println("All settings cleared from NVS. Reloading defaults...");
    
    // Reload with defaults
    loadSettings();
}

// Observer Management

void SettingPersistence::registerObserver(SettingObserver* observer) {
    if (observer) {
        _observers.push_back(observer);
    }
}

void SettingPersistence::unregisterObserver(SettingObserver* observer) {
    auto it = std::find(_observers.begin(), _observers.end(), observer);
    if (it != _observers.end()) {
        _observers.erase(it);
    }
}

void SettingPersistence::notifyObservers(Setting setting) const {
    for (auto observer : _observers) {
        if (observer->isInterestedIn(setting)) {
            observer->markChanged();
        }
    }
}