#include "../WebInterface.hpp"
#include "config/DirectoriesConfig.hpp"

static const char* TAG = "WebInterface";

void WebInterface::handleDeleteJob() {
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

    if (_jobController.isActive() && _jobController.getCurrentFile() == filename)
    {
        _httpServer.send(400, "text/plain", "Cannot delete file - job is currently active");
        return;
    }

    if (_fileManager.deleteFile(PLOTTING_DIRECTORY + filename))
    {
        _httpServer.send(200, "text/plain", "File deleted");
    }
    else
    {
        _httpServer.send(500, "text/plain", "Failed to delete file");
    }
}