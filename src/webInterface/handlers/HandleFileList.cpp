#include "../WebInterface.hpp"
#include "config/directories_config.hpp"

void WebInterface::handleFileList()
{
    auto files = _fileManager.listFiles(PLOTTING_DIRECTORY);

    std::string json = "[";
    for (size_t i = 0; i < files.size(); i++)
    {
        if (i != 0) json += ",";
        json += "\"" + files[i] + "\"";
    }
    json += "]";

    _server.send(200, "application/json", json.c_str());
}
