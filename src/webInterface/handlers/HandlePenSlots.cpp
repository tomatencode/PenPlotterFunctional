#include "../WebInterface.hpp"

#include <cstdio>

void WebInterface::getActivePenSlot() {
    const auto activeSlot = _motionState.getActivePenSlot();
    _server.send(200, "application/json", activeSlot.has_value() ? std::to_string(activeSlot.value()).c_str() : "null");
}

void WebInterface::getPenSlots() {
    const auto penSlots = _runtimeSettings.penSlots();

    char buf[16];
    std::string json = "[";
    for (size_t i = 0; i < penSlots.size(); ++i) {
        const PenSlot& slot = penSlots[i];
        json += "{\"empty\":";
        json += slot.empty ? "true" : "false";
        json += ",\"stroke\":";
        snprintf(buf, sizeof(buf), "%u", (unsigned)slot.stroke);
        json += buf;
        json += ",\"color\":[";
        for (int c = 0; c < 4; ++c) {
            snprintf(buf, sizeof(buf), "%u", (unsigned)slot.color[c]);
            json += buf;
            if (c < 3) json += ",";
        }
        json += "]}";
        if (i + 1 < penSlots.size()) json += ",";
    }
    json += "]";

    _server.send(200, "application/json", json.c_str());
}

void WebInterface::setPenSlots() {
    const std::string body = _server.arg("plain").c_str();

    std::array<PenSlot, NUM_PEN_SLOTS> penSlots{};
    size_t pos = 0;

    for (int i = 0; i < NUM_PEN_SLOTS; ++i) {
        const size_t start = body.find('{', pos);
        const size_t end   = body.find('}', start);
        if (start == std::string::npos || end == std::string::npos) {
            _server.send(400, "text/plain", "Invalid JSON: missing slot object");
            return;
        }

        const std::string obj = body.substr(start, end - start + 1);
        PenSlot& slot = penSlots[i];

        slot.empty = obj.find("\"empty\":true")  != std::string::npos;

        const size_t strokePos = obj.find("\"stroke\":");
        if (strokePos == std::string::npos) {
            _server.send(400, "text/plain", "Invalid JSON: missing 'stroke'");
            return;
        }
        int stroke = 0;
        if (sscanf(obj.c_str() + strokePos + 9, "%d", &stroke) != 1) {
            _server.send(400, "text/plain", "Invalid JSON: bad 'stroke' value");
            return;
        }
        slot.stroke = static_cast<uint8_t>(stroke);

        const size_t colorPos = obj.find("\"color\":[");
        const size_t colorEnd = (colorPos != std::string::npos) ? obj.find(']', colorPos) : std::string::npos;
        if (colorPos == std::string::npos || colorEnd == std::string::npos) {
            _server.send(400, "text/plain", "Invalid JSON: missing 'color'");
            return;
        }
        const std::string colorStr = obj.substr(colorPos + 9, colorEnd - colorPos - 9);

        int r, g, b, a;
        if (sscanf(colorStr.c_str(), "%d,%d,%d,%d", &r, &g, &b, &a) != 4) {
            _server.send(400, "text/plain", "Invalid JSON: color must have 4 components");
            return;
        }
        slot.color[0] = static_cast<uint8_t>(r);
        slot.color[1] = static_cast<uint8_t>(g);
        slot.color[2] = static_cast<uint8_t>(b);
        slot.color[3] = static_cast<uint8_t>(a);

        pos = end + 1;
    }

    _settingPersistence.setPenSlots(penSlots);
    _server.send(200, "text/plain", "OK");
}
