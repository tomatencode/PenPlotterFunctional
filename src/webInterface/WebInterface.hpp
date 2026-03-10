#pragma once

#include <WebServer.h>

class WebInterface
{
public:
    WebInterface();
    void init();
    void update();

private:
    WebServer server;

    void handleFileList();
    void handlePauseJob();
    void handleResumeJob();
    void handleStartJob();
    void handleAbortJob();
    void handleUpload();
};