#include "../WebInterface.hpp"

void WebInterface::handleGetPosition() {
    char buf[64];
    snprintf(buf, sizeof(buf), "{\"x\":%.3f,\"y\":%.3f}", _motionState.getMachineX(), _motionState.getMachineY());
    _httpServer.send(200, "application/json", buf);
}

void WebInterface::handleGetMotionState() {
    const char* motionStateStr;
    switch (_motionState.getState()) {
        case MotionStateType::RUNNING: motionStateStr = "running"; break;
        case MotionStateType::PAUSED:  motionStateStr = "paused";  break;
        default:                       motionStateStr = "idle";    break;
    }

    char buf[64];
    snprintf(buf, sizeof(buf), "{\"motionState\":\"%s\"}", motionStateStr);
    _httpServer.send(200, "application/json", buf);
}

void WebInterface::handleGetActivePenSlot() {
    const auto activeSlot = _motionState.getActivePenSlot();
    _httpServer.send(200, "application/json", activeSlot.has_value() ? std::to_string(activeSlot.value()).c_str() : "null");
}