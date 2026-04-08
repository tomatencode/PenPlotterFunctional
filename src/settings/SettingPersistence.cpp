#include "SettingPersistence.hpp"
#include "defaults/PlottingDefaults.hpp"
#include "defaults/WifiDefaults.hpp"
#include <Preferences.h>
#include <algorithm>
#include <esp_log.h>

static const char* TAG = "SettingPersistence";


void SettingPersistence::init() {
    loadSettings();
}


// ── NVS persist helpers ────────────────────────────────────────────────────

void SettingPersistence::persist(const char* key, float v) {
    Preferences prefs;
    prefs.begin("settings", false);
    prefs.putFloat(key, v);
    prefs.end();
}

void SettingPersistence::persist(const char* key, uint8_t v) {
    Preferences prefs;
    prefs.begin("settings", false);
    prefs.putUChar(key, v);
    prefs.end();
}

void SettingPersistence::persist(const char* key, uint16_t v) {
    Preferences prefs;
    prefs.begin("settings", false);
    prefs.putUShort(key, v);
    prefs.end();
}

void SettingPersistence::persist(const char* key, uint32_t v) {
    Preferences prefs;
    prefs.begin("settings", false);
    prefs.putUInt(key, v);
    prefs.end();
}

void SettingPersistence::persist(const char* key, const std::string& v) {
    Preferences prefs;
    prefs.begin("settings", false);
    prefs.putString(key, v.c_str());
    prefs.end();
}


// ── Setters: clamp/truncate → persist → update RuntimeSettings → notify ───

void SettingPersistence::setSSID(const std::string& value) {
    const std::string clamped = value.substr(0, SETTING_MAX_SSID_LEN);
    persist("ssid", clamped);
    _runtimeSettings.setSSID(clamped);
    notifyObservers(Setting::SSID);
}

void SettingPersistence::setPassword(const std::string& value) {
    const std::string clamped = value.substr(0, SETTING_MAX_PASSWORD_LEN);
    persist("password", clamped);
    _runtimeSettings.setPassword(clamped);
    notifyObservers(Setting::Password);
}

void SettingPersistence::setMdnsName(const std::string& value) {
    const std::string clamped = value.substr(0, SETTING_MAX_MDNS_LEN);
    persist("mdnsName", clamped);
    _runtimeSettings.setMdnsName(clamped);
    notifyObservers(Setting::MdnsName);
}

void SettingPersistence::setDriverCurrent_mA(float value) {
    const float clamped = std::clamp(value, MIN_DRIVER_CURRENT_MA, MAX_DRIVER_CURRENT_MA);
    persist("driverCurrent", clamped);
    _runtimeSettings.setDriverCurrent_mA(clamped);
    notifyObservers(Setting::DriverCurrent);
}

void SettingPersistence::setMicrosteps(float value) {
    const float clamped = std::clamp(value, MIN_MICROSTEPS, MAX_MICROSTEPS);
    persist("microsteps", clamped);
    _runtimeSettings.setMicrosteps(clamped);
    notifyObservers(Setting::Microsteps);
}

void SettingPersistence::setDrawFeedRate_mm_per_s(float value) {
    const float clamped = std::clamp(value, MIN_DRAW_FEED_RATE_MM_PER_S, MAX_DRAW_FEED_RATE_MM_PER_S);
    persist("drawFeed", clamped);
    _runtimeSettings.setDrawFeedRate_mm_per_s(clamped);
    notifyObservers(Setting::DrawFeedRate);
}

void SettingPersistence::setTravelFeedRate_mm_per_s(float value) {
    const float clamped = std::clamp(value, MIN_TRAVEL_FEED_RATE_MM_PER_S, MAX_TRAVEL_FEED_RATE_MM_PER_S);
    persist("travelFeed", clamped);
    _runtimeSettings.setTravelFeedRate_mm_per_s(clamped);
    notifyObservers(Setting::TravelFeedRate);
}

void SettingPersistence::setHomingSpeed_stp_per_s(float value) {
    const float clamped = std::clamp(value, MIN_HOMING_SPEED_STP_PER_S, MAX_HOMING_SPEED_STP_PER_S);
    persist("homingSpeed", clamped);
    _runtimeSettings.setHomingSpeed_stp_per_s(clamped);
    notifyObservers(Setting::HomingSpeed);
}

void SettingPersistence::setHomingBackOffSpeed_stp_per_s(float value) {
    const float clamped = std::clamp(value, MIN_HOMING_BO_SPEED_STP_PER_S, MAX_HOMING_BO_SPEED_STP_PER_S);
    persist("homingBOspd", clamped);
    _runtimeSettings.setHomingBackOffSpeed_stp_per_s(clamped);
    notifyObservers(Setting::HomingBackOffSpeed);
}

void SettingPersistence::setStallguardThreshold(float value) {
    const float clamped = std::clamp(value, MIN_SG_THRESHOLD, MAX_SG_THRESHOLD);
    persist("SGThreshold", clamped);
    _runtimeSettings.setStallguardThreshold(clamped);
    notifyObservers(Setting::StallguardThreshold);
}

void SettingPersistence::setBackOffStepsX(uint16_t value) {
    const uint16_t clamped = std::clamp(value, MIN_BACK_OFF_STEPS, MAX_BACK_OFF_STEPS);
    persist("boStepsX", clamped);
    _runtimeSettings.setBackOffStepsX(clamped);
    notifyObservers(Setting::BackOffStepsX);
}

void SettingPersistence::setBackOffStepsY(uint16_t value) {
    const uint16_t clamped = std::clamp(value, MIN_BACK_OFF_STEPS, MAX_BACK_OFF_STEPS);
    persist("boStepsY", clamped);
    _runtimeSettings.setBackOffStepsY(clamped);
    notifyObservers(Setting::BackOffStepsY);
}

void SettingPersistence::setHomingTimeout_us(uint32_t value) {
    const uint32_t clamped = std::clamp(value, MIN_HOMING_TIMEOUT_US, MAX_HOMING_TIMEOUT_US);
    persist("homingTimeout", clamped);
    _runtimeSettings.setHomingTimeout_us(clamped);
    notifyObservers(Setting::HomingTimeout);
}

void SettingPersistence::setSGCheckInterval_ms(uint16_t value) {
    const uint16_t clamped = std::clamp(value, MIN_SG_CHECK_INTERVAL_MS, MAX_SG_CHECK_INTERVAL_MS);
    persist("SGCheckInt", clamped);
    _runtimeSettings.setSGCheckInterval_ms(clamped);
    notifyObservers(Setting::SGCheckInterval);
}

void SettingPersistence::setSGStartTimeout_ms(uint16_t value) {
    const uint16_t clamped = std::clamp(value, MIN_SG_START_TIMEOUT_MS, MAX_SG_START_TIMEOUT_MS);
    persist("SGStartTO", clamped);
    _runtimeSettings.setSGStartTimeout_ms(clamped);
    notifyObservers(Setting::SGStartTimeout);
}

void SettingPersistence::setSGHistorySize(uint8_t value) {
    const uint8_t clamped = std::clamp(value, MIN_SG_HISTORY_SIZE, MAX_SG_HISTORY_SIZE);
    persist("SGHistSize", clamped);
    _runtimeSettings.setSGHistorySize(clamped);
    notifyObservers(Setting::SGHistorySize);
}

void SettingPersistence::setPenUpAngle_deg(float value) {
    const float clamped = std::clamp(value, MIN_PEN_ANGLE_DEG, MAX_PEN_ANGLE_DEG);
    persist("upAngle", clamped);
    _runtimeSettings.setPenUpAngle_deg(clamped);
    notifyObservers(Setting::PenUpAngle);
}

void SettingPersistence::setPenDownAngle_deg(float value) {
    const float clamped = std::clamp(value, MIN_PEN_ANGLE_DEG, MAX_PEN_ANGLE_DEG);
    persist("downAngle", clamped);
    _runtimeSettings.setPenDownAngle_deg(clamped);
    notifyObservers(Setting::PenDownAngle);
}


// ── Load from NVS (clamped to guard against corrupt stored values) ─────────

void SettingPersistence::loadSettings() {
    Preferences prefs;
    prefs.begin("settings", true);  // read-only

    _runtimeSettings.setSSID(    prefs.getString("ssid",     SSID    ).c_str());
    _runtimeSettings.setPassword(prefs.getString("password", PASSWORD).c_str());
    _runtimeSettings.setMdnsName(prefs.getString("mdnsName", MDNS_NAME).c_str());

    _runtimeSettings.setDriverCurrent_mA(std::clamp(
        prefs.getFloat("driverCurrent", DRIVER_CURRENT_MA), MIN_DRIVER_CURRENT_MA, MAX_DRIVER_CURRENT_MA));
    _runtimeSettings.setMicrosteps(std::clamp(
        prefs.getFloat("microsteps", MICROSTEPS), MIN_MICROSTEPS, MAX_MICROSTEPS));
    _runtimeSettings.setDrawFeedRate_mm_per_s(std::clamp(
        prefs.getFloat("drawFeed", FEED_RATE_DRAW_MM_PER_S), MIN_DRAW_FEED_RATE_MM_PER_S, MAX_DRAW_FEED_RATE_MM_PER_S));
    _runtimeSettings.setTravelFeedRate_mm_per_s(std::clamp(
        prefs.getFloat("travelFeed", FEED_RATE_TRAVEL_MM_PER_S), MIN_TRAVEL_FEED_RATE_MM_PER_S, MAX_TRAVEL_FEED_RATE_MM_PER_S));
    _runtimeSettings.setHomingSpeed_stp_per_s(std::clamp(
        prefs.getFloat("homingSpeed", HOMING_SPEED_STP_PER_S), MIN_HOMING_SPEED_STP_PER_S, MAX_HOMING_SPEED_STP_PER_S));
    _runtimeSettings.setHomingBackOffSpeed_stp_per_s(std::clamp(
        prefs.getFloat("homingBOspd", HOMING_BACK_OFF_SPEED_STP_PER_S), MIN_HOMING_BO_SPEED_STP_PER_S, MAX_HOMING_BO_SPEED_STP_PER_S));
    _runtimeSettings.setStallguardThreshold(std::clamp(
        prefs.getFloat("SGThreshold", STALLGUARD_THRESHOLD), MIN_SG_THRESHOLD, MAX_SG_THRESHOLD));
    _runtimeSettings.setBackOffStepsX(std::clamp(
        prefs.getUShort("boStepsX", BACK_OFF_STEPS_X), MIN_BACK_OFF_STEPS, MAX_BACK_OFF_STEPS));
    _runtimeSettings.setBackOffStepsY(std::clamp(
        prefs.getUShort("boStepsY", BACK_OFF_STEPS_Y), MIN_BACK_OFF_STEPS, MAX_BACK_OFF_STEPS));
    _runtimeSettings.setHomingTimeout_us(std::clamp(
        prefs.getUInt("homingTimeout", HOMING_TIMEOUT_US), MIN_HOMING_TIMEOUT_US, MAX_HOMING_TIMEOUT_US));
    _runtimeSettings.setSGCheckInterval_ms(std::clamp(
        prefs.getUShort("SGCheckInt", SG_CHECK_INTERVAL_MS), MIN_SG_CHECK_INTERVAL_MS, MAX_SG_CHECK_INTERVAL_MS));
    _runtimeSettings.setSGStartTimeout_ms(std::clamp(
        prefs.getUShort("SGStartTO", SG_START_TIMEOUT_MS), MIN_SG_START_TIMEOUT_MS, MAX_SG_START_TIMEOUT_MS));
    _runtimeSettings.setSGHistorySize(std::clamp(
        prefs.getUChar("SGHistSize", SG_HISTORY_SIZE), MIN_SG_HISTORY_SIZE, MAX_SG_HISTORY_SIZE));
    _runtimeSettings.setPenUpAngle_deg(std::clamp(
        prefs.getFloat("upAngle", PEN_UP_DEG), MIN_PEN_ANGLE_DEG, MAX_PEN_ANGLE_DEG));
    _runtimeSettings.setPenDownAngle_deg(std::clamp(
        prefs.getFloat("downAngle", PEN_DOWN_DEG), MIN_PEN_ANGLE_DEG, MAX_PEN_ANGLE_DEG));

    prefs.end();
}


// ── Utility ───────────────────────────────────────────────────────────────

void SettingPersistence::clearAllSettings() {
    Preferences prefs;
    prefs.begin("settings", false);
    prefs.clear();
    prefs.end();

    ESP_LOGI(TAG, "All settings cleared from NVS, reloading defaults");
    loadSettings();
}


// ── Observer Management ───────────────────────────────────────────────────

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