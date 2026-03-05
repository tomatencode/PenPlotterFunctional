#include "WebInterface.hpp"
#include "../app/JobManager.hpp"
#include "../storage/FileSystem.hpp"

#include <WiFi.h>
#include <WebServer.h>
#include <FS.h>

static WebServer server(80);

static const char* ssid = "Plotter";
static const char* password = "12345678";

// ===== Handlers =====

void handleFileList()
{
    auto files = fsListFiles();

    String json = "[";
    for (size_t i = 0; i < files.size(); i++)
    {
        if (i != 0) json += ",";
        json += "\"" + files[i] + "\"";
    }
    json += "]";

    server.send(200, "application/json", json);
}

void handleStartJob()
{
    if (!server.hasArg("file"))
    {
        server.send(400, "text/plain", "Missing 'file' parameter");
        return;
    }

    String filename = server.arg("file");

    if (!fsExists(filename))
    {
        server.send(404, "text/plain", "File not found");
        return;
    }

    jobStart(filename);

    server.send(200, "text/plain", "Job started");
}

void handleStopJob()
{
    jobStop();
    server.send(200, "text/plain", "Job stopped");
}

// Upload handler
void handleUpload()
{
    HTTPUpload& upload = server.upload();

    if (upload.status == UPLOAD_FILE_START)
    {
        String path = "/" + upload.filename;
        Serial.printf("Upload start: %s\n", path.c_str());

        if (fsExists(path)) fsDelete(path);

        File f = fsOpenWrite(path);
        if (!f)
            Serial.println("Failed to open file for writing");

        f.close();
    }
    else if (upload.status == UPLOAD_FILE_WRITE)
    {
        String path = "/" + upload.filename;
        File f = fsOpenWrite(path);
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

// ===== Init =====

void webInit()
{
    WiFi.softAP(ssid, password);

    server.on("/files", HTTP_GET, handleFileList);
    server.on("/start", HTTP_POST, handleStartJob);
    server.on("/stop", HTTP_POST, handleStopJob);
    server.on("/upload", HTTP_POST, [](){}, handleUpload);

    server.begin();
    Serial.println("Web interface started");
}

// ===== Update =====

void webUpdate()
{
    server.handleClient();
}