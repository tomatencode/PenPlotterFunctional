#pragma once

#include "../framework/screen/Screen.hpp"

#include "systemServices/MotionState.hpp"
#include "jobController/JobController.hpp"

namespace ui {
namespace screens {

// The main screen of the UI
class PlottingScreen : public ui::Screen
{
public:
    PlottingScreen(const String& filename, JobController& jobController, MotionState& ms, bool alreadyStarted = false);
};

} // namespace screens
} // namespace ui