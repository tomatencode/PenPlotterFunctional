#include "WebInterface.hpp"

#include <string>
#include "config/job_config.hpp"

void WebInterface::handleFileList()
{
    auto files = _fileManager.listFiles(PLOTTING_DIRECTORY + "/");

    std::string json = "[";
    for (size_t i = 0; i < files.size(); i++)
    {
        if (i != 0) json += ",";
        json += "\"" + files[i] + "\"";
    }
    json += "]";

    server.send(200, "application/json", json.c_str());
}

void WebInterface::handlePauseJob()
{
    _jobController.pause();
    server.send(200, "text/plain", "Job paused");
}

void WebInterface::handleResumeJob()
{
    _jobController.resume();
    server.send(200, "text/plain", "Job resumed");
}

void WebInterface::handleStartJob()
{
    if (!server.hasArg("file"))
    {
        server.send(400, "text/plain", "Missing 'file' parameter");
        return;
    }

    String filename = server.arg("file");

    if (!_fileManager.fileExists(PLOTTING_DIRECTORY + "/" + filename.c_str()))
    {
        server.send(404, "text/plain", "File not found");
        return;
    }

    _jobController.start(filename.c_str());

    server.send(200, "text/plain", "Job started");
}

void WebInterface::handleAbortJob()
{
    _jobController.abort();
    server.send(200, "text/plain", "Job stopped");
}

void WebInterface::handleUpload()
{
    HTTPUpload& upload = server.upload();

    if (upload.status == UPLOAD_FILE_START)
    {
        std::string path = std::string("/") + upload.filename.c_str();
        Serial.printf("Upload start: %s\n", path.c_str());

        if (_fileManager.fileExists(path))
            _fileManager.deleteFile(path);

        File f = _fileManager.openFileWrite(path);
        if (!f)
            Serial.println("Failed to open file for writing");

        f.close();
    }
    else if (upload.status == UPLOAD_FILE_WRITE)
    {
        std::string path = std::string("/") + upload.filename.c_str();
        File f = _fileManager.openFileWrite(path);
        f.write(upload.buf, upload.currentSize);
        f.close();
    }
    else if (upload.status == UPLOAD_FILE_END)
    {
        Serial.printf("Upload finished: %s (%u bytes)\n",
                      upload.filename.c_str(), upload.totalSize);
        server.send(200, "text/plain", "File uploaded");
    }
}
