#include "../WebInterface.hpp"
#include "config/DirectoriesConfig.hpp"

static const char* TAG = "WebInterface";

void WebInterface::handleDeleteJob() {
    if (!_server.hasArg("file"))
    {
        _server.send(400, "text/plain", "Missing 'file' parameter");
        return;
    }

    std::string filename = _server.arg("file").c_str();

    if (!validateFileName(filename) || !filename.ends_with(".gcode"))
    {
        _server.send(400, "text/plain", "Invalid filename");
        return;
    }

    if (!_fileManager.fileExists(PLOTTING_DIRECTORY + filename))
    {
        _server.send(404, "text/plain", "File not found");
        return;
    }

    if (_jobController.isActive() && _jobController.getCurrentFile() == filename)
    {
        _server.send(400, "text/plain", "Cannot delete file - job is currently active");
        return;
    }

    if (_fileManager.deleteFile(PLOTTING_DIRECTORY + filename))
    {
        _server.send(200, "text/plain", "File deleted");
    }
    else
    {
        _server.send(500, "text/plain", "Failed to delete file");
    }
}