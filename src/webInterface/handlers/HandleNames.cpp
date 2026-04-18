#include "../WebInterface.hpp"

void WebInterface::handleGetName() {
    std::string name = _runtimeSettings.name();
    _httpServer.send(200, "text/plain", name.c_str());
}

void WebInterface::changeName() {
    std::string newName = _httpServer.arg("plain").c_str();
    _settingPersistence.setName(newName);
}

void WebInterface::handleGetMDNSName() {
    std::string mdnsName = _runtimeSettings.mdnsName();
    _httpServer.send(200, "text/plain", mdnsName.c_str());
}

void WebInterface::changeMDNSName() {
    std::string newMdnsName = _httpServer.arg("plain").c_str();
    _settingPersistence.setMdnsName(newMdnsName);
}