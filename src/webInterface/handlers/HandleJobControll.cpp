#include "../WebInterface.hpp"
#include "config/DirectoriesConfig.hpp"

void WebInterface::handleStartJob() {
    if (!_server.hasArg("file"))
    {
        _server.send(400, "text/plain", "Missing 'file' parameter");
        return;
    }

    std::string filename = _server.arg("file").c_str();

    if (!_fileManager.fileExists(PLOTTING_DIRECTORY + filename))
    {
        _server.send(404, "text/plain", "File not found");
        return;
    }

    _jobController.start(filename.c_str());
    _server.send(200, "text/plain", "Job started");
}

void WebInterface::handleAbortJob() {
    _jobController.abort();
    _server.send(200, "text/plain", "Job aborted");
}

void WebInterface::handlePauseJob() {
    _jobController.pause();
    _server.send(200, "text/plain", "Job paused");
}

void WebInterface::handleResumeJob() {
    _jobController.resume();
    _server.send(200, "text/plain", "Job resumed");
}