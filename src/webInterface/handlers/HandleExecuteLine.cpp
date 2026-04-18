#include "../WebInterface.hpp"

void WebInterface::handleExecuteLine() {
    std::string line = _httpServer.arg("plain").c_str(); // "plain" = raw request body

    if (line.empty())
    {
        _httpServer.send(400, "text/plain", "Missing line");
        return;
    }

    if (line.length() >= MAX_GCODE_LINE) {
        _httpServer.send(400, "text/plain", "Line too long");
        return;
    }

    auto tokenOpt = _gcodeSender.tryAcquire();
    if (!tokenOpt) {
        _httpServer.send(503, "text/plain", "GCode sender is busy");
        return;
    }

    if (!tokenOpt->send(line)) {
        _httpServer.send(500, "text/plain", "Failed to send GCode line");
        return;
    }

    _httpServer.send(200, "text/plain", "Line queued");
}