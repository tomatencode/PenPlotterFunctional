#include "../WebInterface.hpp"

void WebInterface::handleGetName() {
    std::string name = _runtimeSettings.name();
    _server.send(200, "text/plain", name.c_str());
}

void WebInterface::changeName() {
    std::string newName = _server.arg("plain").c_str();
    _settingPersistence.setName(newName);
}

void WebInterface::changeMDNSName() {
    std::string newMdnsName = _server.arg("plain").c_str();
    _settingPersistence.setMdnsName(newMdnsName);
}