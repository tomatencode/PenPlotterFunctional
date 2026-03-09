#ifndef FOCUS_MANAGER_HPP
#define FOCUS_MANAGER_HPP

#include <cstddef>
#include "../widgetSystem/SelectableWidget.hpp"

class FocusManager
{
public:
    FocusManager();

    // Set the list of selectable widgets for the current screen
    void setWidgets(SelectableWidget* const* widgets, size_t count);

    // Forward input to the currently focused widget
    void handleInput(InputState& input);

private:
    void next();
    void prev();

    SelectableWidget* const* _widgets; // pointer to array of selectable widgets
    size_t _count;                      // number of selectable widgets
    size_t _index;                      // current focused widget index
};

#endif