#pragma once

#include "../framework/screen/Screen.hpp"

#include "jobManager/JobManager.hpp"

namespace ui {
namespace screens {

// The main screen of the UI
class HomeScreen : public ui::Screen
{
public:
    HomeScreen(JobManager& jobManager);
};

} // namespace screens
} // namespace ui