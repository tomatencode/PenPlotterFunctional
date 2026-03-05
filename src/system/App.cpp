#include "App.hpp"

#include "../storage/FileSystem.hpp"
#include "../app/JobManager.hpp"
#include "../app/WebInterface.hpp"
#include "../app/UI.hpp"

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