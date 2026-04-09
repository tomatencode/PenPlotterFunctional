#include "../WebInterface.hpp"

void WebInterface::handlePlotActive() {
    bool active = _jobController.isActive();
    _server.send(200, "text/plain", active ? "true" : "false");
}

void WebInterface::handleGetPlotPaused() {
    bool paused = (_motionState.getState() == MotionStateType::PAUSED);
    _server.send(200, "text/plain", paused ? "true" : "false");
}

void WebInterface::handleGetCurrentPlot() {
    std::string filename = _jobController.getCurrentFile();
    if (filename == "") {
        filename = "No Job";
    }
    _server.send(200, "text/plain", filename.c_str());
}

void WebInterface::handleGetPlotProgress() {
    double progress = _jobController.getProgress();
    char buf[16];
    snprintf(buf, sizeof(buf), "%.2f", progress);
    _server.send(200, "text/plain", buf);
}

void WebInterface::handleGetCurrentLine() {
    uint32_t currentLine = _jobController.getCurrentLine();
    char buf[16];
    snprintf(buf, sizeof(buf), "%u", (unsigned int)currentLine);
    _server.send(200, "text/plain", buf);
}

void WebInterface::handleGetTotalLines() {
    uint32_t totalLines = _jobController.getTotalLines();
    char buf[16];
    snprintf(buf, sizeof(buf), "%u", (unsigned int)totalLines);
    _server.send(200, "text/plain", buf);
}

void WebInterface::handleGetJobStatus() {
    bool active = _jobController.isActive();
    bool paused = (_motionState.getState() == MotionStateType::PAUSED);
    uint32_t currentLine = _jobController.getCurrentLine();
    uint32_t totalLines = _jobController.getTotalLines();
    double progress = _jobController.getProgress();
    std::string filename = _jobController.getCurrentFile();

    char buf[128];
    snprintf(buf, sizeof(buf),
        "{\"active\":%s,\"paused\":%s,\"file\":\"%s\",\"currentLine\":%u,\"totalLines\":%u,\"progress\":%.2f}",
        active ? "true" : "false",
        paused ? "true" : "false",
        filename.c_str(),
        (unsigned int)currentLine,
        (unsigned int)totalLines,
        progress
    );

    _server.send(200, "application/json", buf);
}