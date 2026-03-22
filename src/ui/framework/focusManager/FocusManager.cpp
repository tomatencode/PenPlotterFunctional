#include "FocusManager.hpp"

namespace ui {

FocusManager::FocusManager() : _widgets(), _index(0) {}

void FocusManager::setWidgets(std::vector<ui::widgets::SelectableWidget*> widgets)
{
    _widgets = std::move(widgets);
    _index = 0;

    if (_widgets.empty())
        return;
    
    // find the first enabled widget to focus on
    while (_widgets[_index] == nullptr || !_widgets[_index]->isEnabled())
    {
        _index = (_index + 1) % _widgets.size();
        if (_index == 0) return; // Avoid infinite loop if all widgets are null or hidden
    }

    _widgets[_index]->focus();
}

void FocusManager::handleInput(InputState& input)
{
    if (_widgets.empty() || _widgets[_index] == nullptr) return;

    _widgets[_index]->handleInput(input);

    // Handle encoder rotation after focusable widget has processed input, allowing it to consume rotation if needed
    if (input.encoderDelta > 0)
    {
        next();
    }
    else if (input.encoderDelta < 0)
    {
        prev();
    }
}

// Move focus to the next widget
void FocusManager::next()
{
    if (_widgets.empty()) return;

    if (_widgets[_index] != nullptr)
        _widgets[_index]->unfocus();
    
    _index = (_index + 1) % _widgets.size();

    size_t startIndex = _index;
    while (_widgets[_index] == nullptr || !_widgets[_index]->isEnabled())
    {
        _index = (_index + 1) % _widgets.size();
        if (_index == startIndex) return; // Avoid infinite loop if all widgets are null or hidden
    }
    
    _widgets[_index]->focus();
}

// Move focus to the previous widget
void FocusManager::prev()
{
    if (_widgets.empty()) return;

    if (_widgets[_index] != nullptr)
        _widgets[_index]->unfocus();

    _index = (_index + _widgets.size() - 1) % _widgets.size();
    
    size_t startIndex = _index;
    while (_widgets[_index] == nullptr || !_widgets[_index]->isEnabled())
    {
        _index = (_index + _widgets.size() - 1) % _widgets.size();
        if (_index == startIndex) return; // Avoid infinite loop if all widgets are null or hidden
    }
    
        _widgets[_index]->focus();
}

void FocusManager::refresh() {
    if (_widgets.empty()) return;
    // if current invalid, skip
    if (_index < _widgets.size() && _widgets[_index] && _widgets[_index]->isEnabled()) return;
    if (_index < _widgets.size() && _widgets[_index]) _widgets[_index]->unfocus();

    size_t startIndex = _index;
    while (_widgets[_index] == nullptr || !_widgets[_index]->isEnabled())
    {
        _index = (_index + _widgets.size() - 1) % _widgets.size();
        if (_index == startIndex) return; // Avoid infinite loop if all widgets are null or hidden
    }

    _widgets[_index]->focus();
}

} // namespace ui