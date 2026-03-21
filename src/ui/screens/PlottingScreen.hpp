#pragma once

#include "../framework/screen/Screen.hpp"

namespace ui {
namespace screens {

// The main screen of the UI
class PlottingScreen : public ui::Screen
{
public:
    PlottingScreen(const String& filename);
};

} // namespace screens
} // namespace ui