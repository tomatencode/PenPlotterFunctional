#pragma once

#include "../framework/screen/Screen.hpp"

namespace ui {
namespace screens {

// The main screen of the UI
class FileDetailsScreen : public ui::Screen
{
public:
    FileDetailsScreen(const String& filename);
};

} // namespace screens
} // namespace ui