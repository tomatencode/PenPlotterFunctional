#include "../WebInterface.hpp"

void WebInterface::handleExecuteLine()
{
    std::string line = _server.arg("plain").c_str(); // "plain" = raw request body

    if (line.empty())
    {
        _server.send(400, "text/plain", "Missing line");
        return;
    }

    if (line.length() >= MAX_GCODE_LINE) {
        _server.send(400, "text/plain", "Line too long");
        return;
    }

    auto tokenOpt = _gcodeSender.tryAcquire();
    if (!tokenOpt) {
        _server.send(503, "text/plain", "GCode sender is busy");
        return;
    }

    if (!tokenOpt->send(line)) {
        _server.send(500, "text/plain", "Failed to send GCode line");
        return;
    }

    _server.send(200, "text/plain", "Line queued");
}