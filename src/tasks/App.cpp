#include "App.hpp"

#include "../storage/FileSystem.hpp"
#include "../control/JobManager.hpp"
#include "../control/WebInterface.hpp"
#include "../control/UI.hpp"

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