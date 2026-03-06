#include "App.hpp"

#include "../storage/FileSystem.hpp"
#include "../jobManager/JobManager.hpp"
#include "../interface/WebInterface.hpp"
#include "../interface/UI.hpp"

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