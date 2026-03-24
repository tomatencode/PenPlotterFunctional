#pragma once

#include "../framework/screen/Screen.hpp"

#include "systemServices/MotionStateManager.hpp"
#include "jobManager/JobManager.hpp"

namespace ui {
namespace screens {

// The main screen of the UI
class FilesScreen : public ui::Screen
{
public:
    FilesScreen(JobManager& jobManager, MotionStateManager& ms);

    void onEnter() override;
};

} // namespace screens
} // namespace ui