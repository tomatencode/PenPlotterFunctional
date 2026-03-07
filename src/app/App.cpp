#include "App.hpp"

#include "storage/FileSystem.hpp"
#include "jobManager/JobManager.hpp"
#include "webInterface/WebInterface.hpp"
#include "ui/UI.hpp"

JobManager jobManager;

void appInit()
{
    fsInit();
    webInit();
    uiInit();
}

void appUpdate()
{
    webUpdate();
    uiUpdate();
    jobManager.jobManagerUpdate();
}