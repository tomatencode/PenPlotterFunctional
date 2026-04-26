#include "../WebInterface.hpp"
#include "config/DirectoriesConfig.hpp"

void WebInterface::handleGetFileInfo() {
    if (!_httpServer.hasArg("file"))
    {
        _httpServer.send(400, "text/plain", "Missing 'file' parameter");
        return;
    }

    std::string filename = _httpServer.arg("file").c_str();

    if (!validateFileName(filename) || !filename.ends_with(".gcode"))
    {
        _httpServer.send(400, "text/plain", "Invalid filename");
        return;
    }
    if (!_fileManager.fileExists(PLOTTING_DIRECTORY + filename))
    {
        _httpServer.send(404, "text/plain", "File not found");
        return;
    }

    int totalLines = 0;
    File file = _fileManager.openFileRead(PLOTTING_DIRECTORY + filename);
    if (file) {
        while (file.available()) {
            if (file.read() == '\n') {
                totalLines++;
            }
        }
        file.close();
    }

    std::string jsonResponse = "{";
    jsonResponse += "\"lines\":" + std::to_string(totalLines) + ",";
    jsonResponse += "\"sizeBytes\":" + std::to_string(_fileManager.getFileSize(PLOTTING_DIRECTORY + filename));
    jsonResponse += "}";

    _httpServer.send(200, "application/json", jsonResponse.c_str());
}