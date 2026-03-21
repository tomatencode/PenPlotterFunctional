#include "WebInterface.hpp"
#include "jobManager/JobManager.hpp"
#include "storage/FileSystem.hpp"
#include "applicationManager/ApplicationManager.hpp"

void WebInterface::handleFileList()
{
    auto files = storage::fsListFiles();

    String json = "[";
    for (size_t i = 0; i < files.size(); i++)
    {
        if (i != 0) json += ",";
        json += "\"" + files[i] + "\"";
    }
    json += "]";

    server.send(200, "application/json", json);
}

void WebInterface::handlePauseJob()
{
    jobManager.pause();
    server.send(200, "text/plain", "Job paused");
}

void WebInterface::handleResumeJob()
{
    jobManager.resume();
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

    if (!storage::fsExists(filename))
    {
        server.send(404, "text/plain", "File not found");
        return;
    }

    jobManager.start(filename);

    server.send(200, "text/plain", "Job started");
}

void WebInterface::handleAbortJob()
{
    jobManager.abort();
    server.send(200, "text/plain", "Job stopped");
}

void WebInterface::handleUpload()
{
    HTTPUpload& upload = server.upload();

    if (upload.status == UPLOAD_FILE_START)
    {
        String path = "/" + upload.filename;
        Serial.printf("Upload start: %s\n", path.c_str());

        if (storage::fsExists(path)) storage::fsDelete(path);

        File f = storage::fsOpenWrite(path);
        if (!f)
            Serial.println("Failed to open file for writing");

        f.close();
    }
    else if (upload.status == UPLOAD_FILE_WRITE)
    {
        String path = "/" + upload.filename;
        File f = storage::fsOpenWrite(path);
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
