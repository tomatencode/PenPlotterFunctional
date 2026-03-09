#include "FocusManager.hpp"

FocusManager::FocusManager() : _widgets(nullptr), _count(0), _index(0) {}

void FocusManager::setWidgets(SelectableWidget* const* widgets, size_t count)
{
    _widgets = widgets;
    _count = count;
    _index = 0;

    // Give focus to the first widget if available
    if(_count > 0)
        _widgets[_index]->focus();
}

void FocusManager::handleInput(InputState& input)
{
    if(_count == 0) return;

    _widgets[_index]->handleInput(input);

    // Handle encoder rotation after focusable widget has processed input, allowing it to consume rotation if needed
    if (input.encoderDelta > 0)
    {
        for (size_t i = 0; i < input.encoderDelta ; i++) {
            next();
        }
    }
    else if (input.encoderDelta < 0)
    {
        for (size_t i = 0; i < -input.encoderDelta ; i++) {
            prev();
        }
    }
}

// Move focus to the next widget
void FocusManager::next()
{
    if(_count == 0) return;

    _widgets[_index]->unfocus();
    _index = (_index + 1) % _count;
    _widgets[_index]->focus();
}

// Move focus to the previous widget
void FocusManager::prev()
{
    if(_count == 0) return;

    _widgets[_index]->unfocus();
    _index = (_index + _count - 1) % _count;
    _widgets[_index]->focus();
}