#include "../WebInterface.hpp"
#include "config/DirectoriesConfig.hpp"

void WebInterface::handleStartJob() {
    if (!_httpServer.hasArg("file"))
    {
        _httpServer.send(400, "text/plain", "Missing 'file' parameter");
        return;
    }

    std::string filename = _httpServer.arg("file").c_str();

    if (!_fileManager.fileExists(PLOTTING_DIRECTORY + filename))
    {
        _httpServer.send(404, "text/plain", "File not found");
        return;
    }

    _jobController.start(filename.c_str());
    _httpServer.send(200, "text/plain", "Job started");
}

void WebInterface::handleAbortJob() {
    _jobController.abort();
    _httpServer.send(200, "text/plain", "Job aborted");
}

void WebInterface::handlePauseJob() {
    _jobController.pause();
    _httpServer.send(200, "text/plain", "Job paused");
}

void WebInterface::handleResumeJob() {
    _jobController.resume();
    _httpServer.send(200, "text/plain", "Job resumed");
}