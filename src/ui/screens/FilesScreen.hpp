#pragma once

#include "../framework/screen/Screen.hpp"

#include "jobManager/JobManager.hpp"

namespace ui {
namespace screens {

// The main screen of the UI
class FilesScreen : public ui::Screen
{
public:
    FilesScreen(JobManager& jobManager);

    void onEnter() override;
};

} // namespace screens
} // namespace ui