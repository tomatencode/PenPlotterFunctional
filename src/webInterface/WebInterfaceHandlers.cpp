#include "WebInterface.hpp"
#include "config/job_config.hpp"

#include <string>

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

void WebInterface::handlePauseJob()
{
    _jobController.pause();
    _server.send(200, "text/plain", "Job paused");
}

void WebInterface::handleResumeJob()
{
    _jobController.resume();
    _server.send(200, "text/plain", "Job resumed");
}

void WebInterface::handleStartJob()
{
    if (!_server.hasArg("file"))
    {
        _server.send(400, "text/plain", "Error: Missing 'file' parameter");
        return;
    }

    String filename = _server.arg("file");
    std::string filepath = PLOTTING_DIRECTORY + filename.c_str();

    if (!_fileManager.fileExists(filepath))
    {
        _server.send(404, "text/plain", "Error: File not found");
        return;
    }

    _jobController.start(filename.c_str());
    _server.send(200, "text/plain", "Job started");
}

void WebInterface::handleAbortJob()
{
    _jobController.abort();
    _server.send(200, "text/plain", "Job aborted");
}

void WebInterface::handleUpload()
{
    HTTPUpload& upload = _server.upload();

    if (upload.status == UPLOAD_FILE_START)
    {
        std::string path = PLOTTING_DIRECTORY + upload.filename.c_str();
        Serial.printf("WebInterface: Upload start - %s\n", path.c_str());

        if (_fileManager.fileExists(path))
            _fileManager.deleteFile(path);

        File f = _fileManager.openFileWrite(path);
        if (!f)
            Serial.println("WebInterface: ERROR - Failed to open file for writing");

        f.close();
    }
    else if (upload.status == UPLOAD_FILE_WRITE)
    {
        std::string path = PLOTTING_DIRECTORY + upload.filename.c_str();
        File f = _fileManager.openFileWrite(path);
        f.write(upload.buf, upload.currentSize);
        f.close();
    }
    else if (upload.status == UPLOAD_FILE_END)
    {
        Serial.printf("WebInterface: Upload complete - %s (%u bytes)\n",
                      upload.filename.c_str(), upload.totalSize);
        _server.send(200, "text/plain", "File uploaded successfully");
    }
}
