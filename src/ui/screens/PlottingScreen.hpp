#pragma once

#include "../framework/screen/Screen.hpp"

#include "systemServices/MotionStateManager.hpp"
#include "jobManager/JobManager.hpp"

namespace ui {
namespace screens {

// The main screen of the UI
class PlottingScreen : public ui::Screen
{
public:
    /**
     * @param filename The name of the file to plot
     * @param jobManager Reference to job manager
     * @param ms Reference to motion state manager
     * @param alreadyStarted If true, skips calling jobManager.start() (job already running from observer)
     */
    PlottingScreen(const String& filename, JobManager& jobManager, MotionStateManager& ms, bool alreadyStarted = false);
};

} // namespace screens
} // namespace ui