#include "App.hpp"

#include "../storage/FileSystem.hpp"
#include "JobManager.hpp"
#include "WebInterface.hpp"
#include "UI.hpp"

void appInit()
{
    fsInit();
    jobManagerInit();
    webInit();
    uiInit();
}

void appUpdate()
{
    webUpdate();
    uiUpdate();
    jobManagerUpdate();
}