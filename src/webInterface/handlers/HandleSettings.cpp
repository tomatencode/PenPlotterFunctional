#include "../WebInterface.hpp"

#include <string>
#include <functional>
#include <cstdlib>
#include <cstdio>

static const char* TAG = "WebInterface";


// Helper
bool WebInterface::penSlotsFromJson(const std::string& v, std::array<PenSlot, NUM_PEN_SLOTS>& penSlots) const
{
    size_t pos = 0;
    for (int i = 0; i < NUM_PEN_SLOTS; ++i) {
        const size_t start = v.find('{', pos);
        const size_t end   = v.find('}', start);
        if (start == std::string::npos || end == std::string::npos) return false;
        const std::string obj = v.substr(start, end - start + 1);

        PenSlot& slot = penSlots[i];
        slot.empty = obj.find("\"empty\":true") != std::string::npos;

        const size_t strokePos = obj.find("\"stroke\":");
        if (strokePos == std::string::npos) return false;
        int stroke = 0;
        if (sscanf(obj.c_str() + strokePos + 9, "%d", &stroke) != 1) return false;
        slot.stroke = static_cast<uint8_t>(stroke);

        const size_t colorPos = obj.find("\"color\":[");
        const size_t colorEnd = (colorPos != std::string::npos) ? obj.find(']', colorPos) : std::string::npos;
        if (colorPos == std::string::npos || colorEnd == std::string::npos) return false;
        const std::string colorStr = obj.substr(colorPos + 9, colorEnd - colorPos - 9);
        int r, g, b, a;
        if (sscanf(colorStr.c_str(), "%d,%d,%d,%d", &r, &g, &b, &a) != 4) return false;
        slot.color[0] = static_cast<uint8_t>(r);
        slot.color[1] = static_cast<uint8_t>(g);
        slot.color[2] = static_cast<uint8_t>(b);
        slot.color[3] = static_cast<uint8_t>(a);

        pos = end + 1;
    }
    return true;
}

std::string WebInterface::penSlotsToJson() const
{
    std::string json = "[";
    const auto slots = _runtimeSettings.penSlots();
    char buf[64];
    for (size_t i = 0; i < slots.size(); ++i) {
        const PenSlot& slot = slots[i];
        snprintf(buf, sizeof(buf),
            "{\"empty\":%s,\"stroke\":%u,\"color\":[%u,%u,%u,%u]}",
            slot.empty ? "true" : "false",
            (unsigned)slot.stroke,
            (unsigned)slot.color[0], (unsigned)slot.color[1],
            (unsigned)slot.color[2], (unsigned)slot.color[3]);
        json += buf;
        if (i + 1 < slots.size()) json += ",";
    }
    json += "]";
    return json;
}

// GET /setting?key=...
void WebInterface::handleGetSetting()
{
    if (!_httpServer.hasArg("key")) {
        _httpServer.send(400, "text/plain", "Missing 'key' parameter");
        return;
    }

    const std::string key = _httpServer.arg("key").c_str();

    struct Entry {
        const char* key;
        std::function<std::string()> getter;
    };

    char buf[64];
    const Entry table[] = {
        {"mdnsName",            [this]()                    { return _runtimeSettings.mdnsName(); }},
        {"driverCurrent",       [this, &buf]() -> std::string { snprintf(buf, sizeof(buf), "%.2f", _runtimeSettings.driverCurrent_mA());              return buf; }},
        {"microsteps",          [this, &buf]() -> std::string { snprintf(buf, sizeof(buf), "%.2f", _runtimeSettings.microsteps());                    return buf; }},
        {"drawFeedRate",        [this, &buf]() -> std::string { snprintf(buf, sizeof(buf), "%.2f", _runtimeSettings.drawFeedRate_mm_per_s());          return buf; }},
        {"travelFeedRate",      [this, &buf]() -> std::string { snprintf(buf, sizeof(buf), "%.2f", _runtimeSettings.travelFeedRate_mm_per_s());        return buf; }},
        {"homingSpeed",         [this, &buf]() -> std::string { snprintf(buf, sizeof(buf), "%.2f", _runtimeSettings.homingSpeed_stp_per_s());          return buf; }},
        {"homingBackOffSpeed",  [this, &buf]() -> std::string { snprintf(buf, sizeof(buf), "%.2f", _runtimeSettings.homingBackOffSpeed_stp_per_s());   return buf; }},
        {"stallguardThreshold", [this, &buf]() -> std::string { snprintf(buf, sizeof(buf), "%.2f", _runtimeSettings.stallguardThreshold());            return buf; }},
        {"backOffStepsX",       [this, &buf]() -> std::string { snprintf(buf, sizeof(buf), "%u",   (unsigned)_runtimeSettings.backOffStepsX());        return buf; }},
        {"backOffStepsY",       [this, &buf]() -> std::string { snprintf(buf, sizeof(buf), "%u",   (unsigned)_runtimeSettings.backOffStepsY());        return buf; }},
        {"homingTimeout",       [this, &buf]() -> std::string { snprintf(buf, sizeof(buf), "%u",   (unsigned)_runtimeSettings.homingTimeout_us());     return buf; }},
        {"sgCheckInterval",     [this, &buf]() -> std::string { snprintf(buf, sizeof(buf), "%u",   (unsigned)_runtimeSettings.sgCheckInterval_ms());   return buf; }},
        {"sgStartTimeout",      [this, &buf]() -> std::string { snprintf(buf, sizeof(buf), "%u",   (unsigned)_runtimeSettings.sgStartTimeout_ms());    return buf; }},
        {"sgHistorySize",       [this, &buf]() -> std::string { snprintf(buf, sizeof(buf), "%u",   (unsigned)_runtimeSettings.sgHistorySize());        return buf; }},
        {"penUpAngle",          [this, &buf]() -> std::string { snprintf(buf, sizeof(buf), "%.2f", _runtimeSettings.penUpAngle_deg());                 return buf; }},
        {"penDownAngle",        [this, &buf]() -> std::string { snprintf(buf, sizeof(buf), "%.2f", _runtimeSettings.penDownAngle_deg());               return buf; }},
        {"penSlots",            [this]()       -> std::string { return penSlotsToJson(); }},
    };

    for (const auto& entry : table) {
        if (key == entry.key) {
            _httpServer.send(200, "text/plain", entry.getter().c_str());
            return;
        }
    }

    _httpServer.send(404, "text/plain", "Unknown setting key");
}

// POST /setting?key=...&value=...
void WebInterface::handleSetSetting()
{
    if (!_httpServer.hasArg("key") || !_httpServer.hasArg("value")) {
        _httpServer.send(400, "text/plain", "Missing 'key' or 'value' parameter");
        return;
    }

    const std::string key   = _httpServer.arg("key").c_str();
    const std::string value = _httpServer.arg("value").c_str();

    struct Entry {
        const char* key;
        std::function<void(const std::string&)> setter;
    };

    const Entry table[] = {
        {"mdnsName",            [this](const std::string& v){ _settingPersistence.setMdnsName(v); }},
        {"driverCurrent",       [this](const std::string& v){ _settingPersistence.setDriverCurrent_mA(atof(v.c_str())); }},
        {"microsteps",          [this](const std::string& v){ _settingPersistence.setMicrosteps(atof(v.c_str())); }},
        {"drawFeedRate",        [this](const std::string& v){ _settingPersistence.setDrawFeedRate_mm_per_s(atof(v.c_str())); }},
        {"travelFeedRate",      [this](const std::string& v){ _settingPersistence.setTravelFeedRate_mm_per_s(atof(v.c_str())); }},
        {"homingSpeed",         [this](const std::string& v){ _settingPersistence.setHomingSpeed_stp_per_s(atof(v.c_str())); }},
        {"homingBackOffSpeed",  [this](const std::string& v){ _settingPersistence.setHomingBackOffSpeed_stp_per_s(atof(v.c_str())); }},
        {"stallguardThreshold", [this](const std::string& v){ _settingPersistence.setStallguardThreshold(atof(v.c_str())); }},
        {"backOffStepsX",       [this](const std::string& v){ _settingPersistence.setBackOffStepsX(static_cast<uint16_t>(atoi(v.c_str()))); }},
        {"backOffStepsY",       [this](const std::string& v){ _settingPersistence.setBackOffStepsY(static_cast<uint16_t>(atoi(v.c_str()))); }},
        {"homingTimeout",       [this](const std::string& v){ _settingPersistence.setHomingTimeout_us(static_cast<uint32_t>(atol(v.c_str()))); }},
        {"sgCheckInterval",     [this](const std::string& v){ _settingPersistence.setSGCheckInterval_ms(static_cast<uint16_t>(atoi(v.c_str()))); }},
        {"sgStartTimeout",      [this](const std::string& v){ _settingPersistence.setSGStartTimeout_ms(static_cast<uint16_t>(atoi(v.c_str()))); }},
        {"sgHistorySize",       [this](const std::string& v){ _settingPersistence.setSGHistorySize(static_cast<uint8_t>(atoi(v.c_str()))); }},
        {"penUpAngle",          [this](const std::string& v){ _settingPersistence.setPenUpAngle_deg(atof(v.c_str())); }},
        {"penDownAngle",        [this](const std::string& v){ _settingPersistence.setPenDownAngle_deg(atof(v.c_str())); }},
        {"penSlots",            [this](const std::string& v){
            std::array<PenSlot, NUM_PEN_SLOTS> penSlots{};
            if (!penSlotsFromJson(v, penSlots)) return;
            _settingPersistence.setPenSlots(penSlots);
        }},
    };

    for (const auto& entry : table) {
        if (key == entry.key) {
            ESP_LOGI(TAG, "Attempting to update setting '%s' to '%s'", key.c_str(), value.c_str());
            entry.setter(value);
            _httpServer.send(200, "text/plain", "OK");
            return;
        }
    }

    _httpServer.send(404, "text/plain", "Unknown setting key");
}

// GET /settings  (all settings as JSON)
std::string WebInterface::buildAllSettingsJson()
{
    const auto& s = _runtimeSettings;
    char buf[64];
    std::string json = "{";

    json += "\"mdnsName\":\"" + s.mdnsName() + "\",";

    auto appendFloat = [&](const char* key, float value) {
        snprintf(buf, sizeof(buf), "\"%.2f\"", value);
        json += '"'; json += key; json += "\":";
        snprintf(buf, sizeof(buf), "%.2f", value);
        json += buf; json += ",";
    };
    auto appendUint = [&](const char* key, unsigned value) {
        json += '"'; json += key; json += "\":";
        snprintf(buf, sizeof(buf), "%u", value);
        json += buf; json += ",";
    };

    appendFloat("driverCurrent",       s.driverCurrent_mA());
    appendFloat("microsteps",          s.microsteps());
    appendFloat("drawFeedRate",        s.drawFeedRate_mm_per_s());
    appendFloat("travelFeedRate",      s.travelFeedRate_mm_per_s());
    appendFloat("homingSpeed",         s.homingSpeed_stp_per_s());
    appendFloat("homingBackOffSpeed",  s.homingBackOffSpeed_stp_per_s());
    appendFloat("stallguardThreshold", s.stallguardThreshold());
    appendUint ("backOffStepsX",       (unsigned)s.backOffStepsX());
    appendUint ("backOffStepsY",       (unsigned)s.backOffStepsY());
    appendUint ("homingTimeout",       (unsigned)s.homingTimeout_us());
    appendUint ("sgCheckInterval",     (unsigned)s.sgCheckInterval_ms());
    appendUint ("sgStartTimeout",      (unsigned)s.sgStartTimeout_ms());
    appendUint ("sgHistorySize",       (unsigned)s.sgHistorySize());
    appendFloat("penUpAngle",          s.penUpAngle_deg());
    appendFloat("penDownAngle",        s.penDownAngle_deg());

    json += "\"penSlots\":" + penSlotsToJson();
    json += "}";
    return json;
}

void WebInterface::handleGetAllSettings()
{
    const std::string json = buildAllSettingsJson();
    _httpServer.send(200, "application/json", json.c_str());
}
