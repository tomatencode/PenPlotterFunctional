#ifndef FOCUS_MANAGER_HPP
#define FOCUS_MANAGER_HPP

#include <cstddef>
#include "../widgetSystem/SelectableWidget.hpp"

constexpr size_t MAX_SELECTABLE_WIDGETS = 16;

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

    SelectableWidget* _widgets[MAX_SELECTABLE_WIDGETS]; // Own storage for widgets
    size_t _count;                      // number of selectable widgets
    size_t _index;                      // current focused widget index
};

#endif