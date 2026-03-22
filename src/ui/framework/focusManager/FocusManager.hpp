#pragma once

#include <cstddef>
#include <vector>
#include "../widgets/core/SelectableWidget.hpp"

namespace ui {

class FocusManager
{
public:
    FocusManager();

    // Set the list of selectable widgets for the current screen (moved in)
    void setWidgets(std::vector<widgets::SelectableWidget*> widgets, uint8_t firstFocused = 0);

    // Forward input to the currently focused widget
    void handleInput(InputState& input);

    void refresh();

private:
    void next();
    void prev();

    std::vector<widgets::SelectableWidget*> _widgets; // Own storage for widgets
    size_t _index = 0;                       // current focused widget index
};

} // namespace ui