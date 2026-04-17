#include "../WebInterface.hpp"

void WebInterface::changeName() {
    std::string newName = _server.arg("plain").c_str();
    _settingPersistence.setName(newName);
}

void WebInterface::changeMDNSName() {
    std::string newMdnsName = _server.arg("plain").c_str();
    _settingPersistence.setMdnsName(newMdnsName);
}