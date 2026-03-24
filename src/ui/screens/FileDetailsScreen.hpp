#pragma once

#include "../framework/screen/Screen.hpp"

#include "systemServices/MotionStateManager.hpp"
#include "storage/FileManager.hpp"
#include "jobManager/JobManager.hpp"

namespace ui {
namespace screens {

// The main screen of the UI
class FileDetailsScreen : public ui::Screen
{
public:
    FileDetailsScreen(const String& filename, JobManager& jobManager, MotionStateManager& ms, FileManager& fileManager);
};

} // namespace screens
} // namespace ui