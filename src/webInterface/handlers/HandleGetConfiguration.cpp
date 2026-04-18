#include "../WebInterface.hpp"
#include "config/HardwareConfig.hpp"
#include "config/VersionConfig.hpp"

void WebInterface::handleGetItteration() {
    char buf[64];
    snprintf(buf, sizeof(buf), "{\"iteration\":%u}", HARDWARE_ITTERATION);
    _server.send(200, "application/json", buf);
}

void WebInterface::handleGetFirmwareVersion() {
    char buf[64];
    snprintf(buf, sizeof(buf), "{\"firmwareVersion\":\"%u.%u.%u\"}", 
        (FIRMWARE_VERSION >> 16) & 0xFF, 
        (FIRMWARE_VERSION >> 8) & 0xFF, 
        FIRMWARE_VERSION & 0xFF);
    _server.send(200, "application/json", buf);
}

void WebInterface::handleGetWorkspace() {
    char buf[64];
    snprintf(buf, sizeof(buf), "{\"x\":%.3f,\"y\":%.3f}", MAX_X_MM, MAX_Y_MM);
    _server.send(200, "application/json", buf);
}