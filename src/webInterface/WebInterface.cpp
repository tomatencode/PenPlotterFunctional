#include "WebInterface.hpp"
#include "jobManager/JobManager.hpp"
#include "storage/FileSystem.hpp"
#include "systemServices/shared/MotionCommand.hpp"

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include <FS.h>
#include "app/App.hpp"

static WebServer server(80);

// Replace these with your home WiFi credentials
static const char* ssid = "Thomas_Wifi";
static const char* password = "TotallyHarmless";

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

void handlePauseJob()
{
    jobManager.pause();
    server.send(200, "text/plain", "Job paused");
}

void handleResumeJob()
{
    jobManager.resume();
    server.send(200, "text/plain", "Job resumed");
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

    jobManager.start(filename);

    server.send(200, "text/plain", "Job started");
}

void handleAbortJob()
{
    jobManager.abort();
    server.send(200, "text/plain", "Job stopped");
}

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
    Serial.print("Connecting to WiFi ");
    Serial.println(ssid);


    int tries = 0;
    for (tries = 0; tries < 3; tries++)
    {
        WiFi.begin(ssid, password);

        // Wait for connection
        float secs = 0;
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
            secs += 0.5;
            if (secs > 5)
            {   
                if (tries < 2) Serial.println("\nRetrying WiFi connection...");
                else Serial.println("\nFailed to connect to WiFi after 3 attempts");
                return;
            }
        }
    }

    Serial.println("");
    Serial.print("Connected! IP address: ");
    Serial.println(WiFi.localIP());

    // ===== mDNS =====
    if (!MDNS.begin("esp32")) // hostname = esp32.local
    {
        Serial.println("Error starting mDNS");
    }
    else
    {
        Serial.println("mDNS started: http://esp32.local");
    }

    // HTTP server routes
    server.on("/files", HTTP_GET, handleFileList);
    server.on("/start", HTTP_POST, handleStartJob);
    server.on("/abort", HTTP_POST, handleAbortJob);
    server.on("/upload", HTTP_POST, [](){}, handleUpload);
    server.on("/pause", HTTP_POST, handlePauseJob);
    server.on("/resume", HTTP_POST, handleResumeJob);

    server.begin();
    Serial.println("Web interface started");
}

void webUpdate()
{
    server.handleClient();
}