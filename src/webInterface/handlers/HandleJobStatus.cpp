#include "../WebInterface.hpp"

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