#pragma once

#include <functional>

#include "../framework/screen/Screen.hpp"

#include "systemServices/MotionState.hpp"
#include "jobController/JobController.hpp"

namespace ui {
namespace screens {

// The main screen of the UI
class HomeScreen : public ui::Screen
{
public:
    HomeScreen(JobController& jobController,
               MotionState& motionState,
               FileManager& fileManager,
               std::function<bool()> wifiStatusProvider
               );
};

} // namespace screens
} // namespace ui