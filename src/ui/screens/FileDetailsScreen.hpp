#pragma once

#include "../framework/screen/Screen.hpp"

#include "systemServices/MotionState.hpp"
#include "storage/FileManager.hpp"
#include "jobController/JobController.hpp"

namespace ui {
namespace screens {

// The main screen of the UI
class FileDetailsScreen : public ui::Screen
{
public:
    FileDetailsScreen(const String& filename,
                      JobController& jobController,
                      MotionState& motionState,
                      FileManager& fileManager,
                      std::function<bool()> wifiStatusProvider
    );
};

} // namespace screens
} // namespace ui