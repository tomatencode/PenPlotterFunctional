#pragma once

#include <cstddef>
#include <vector>
#include "../widgets/core/Selectable.hpp"

namespace ui {

class FocusManager
{
public:
    FocusManager();

    // Set the list of selectable widgets for the current screen (moved in)
    void setWidgets(std::vector<widgets::Selectable*> widgets, uint8_t firstFocused = 0);

    // Forward input to the currently focused widget
    void handleInput(InputState& input);

    // make sure selected widget is still enabled
    void refresh();

private:
    void next();
    void prev();

    std::vector<widgets::Selectable*> _widgets; // Own storage for widgets
    size_t _selecedIndx = 0;                    // current focused widget index
};

} // namespace ui