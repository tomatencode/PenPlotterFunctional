#include "FocusManager.hpp"

namespace ui {

FocusManager::FocusManager() : _widgets(), _selecedIndx(0) {}

void FocusManager::setWidgets(std::vector<ui::widgets::Selectable*> widgets, uint8_t firstFocused)
{
    _widgets = std::move(widgets);

    if (_widgets.empty()) return;

    firstFocused = std::min(static_cast<size_t>(firstFocused), _widgets.size() - 1);

    _selecedIndx = firstFocused;
    
    // find the first enabled widget to focus on
    while (_widgets[_selecedIndx] == nullptr || !_widgets[_selecedIndx]->isEnabled())
    {
        _selecedIndx = (_selecedIndx + 1) % _widgets.size();
        if (_selecedIndx == firstFocused) return; // Avoid infinite loop if all widgets are null or hidden
    }

    _widgets[_selecedIndx]->focus();
}

void FocusManager::handleInput(InputState& input)
{
    if (_widgets.empty() || _widgets[_selecedIndx] == nullptr) return;

    _widgets[_selecedIndx]->handleInput(input);

    // Handle encoder rotation after focusable widget has processed input, allowing it to consume rotation if needed
    if (input.encoderDelta > 0)
    {
        next();
    }
    else if (input.encoderDelta < 0)
    {
        prev();
    }

    input.encoderDelta = 0; // Consume encoder input
}

// Move focus to the next widget
void FocusManager::next()
{
    if (_widgets.empty()) return;

    if (_widgets[_selecedIndx] != nullptr)
        _widgets[_selecedIndx]->unfocus();
    
    _selecedIndx = (_selecedIndx + 1) % _widgets.size();

    size_t startIndex = _selecedIndx;
    while (_widgets[_selecedIndx] == nullptr || !_widgets[_selecedIndx]->isEnabled())
    {
        _selecedIndx = (_selecedIndx + 1) % _widgets.size();
        if (_selecedIndx == startIndex) return; // Avoid infinite loop if all widgets are null or hidden
    }
    
    _widgets[_selecedIndx]->focus();
}

// Move focus to the previous widget
void FocusManager::prev()
{
    if (_widgets.empty()) return;

    if (_widgets[_selecedIndx] != nullptr)
        _widgets[_selecedIndx]->unfocus();

    _selecedIndx = (_selecedIndx + _widgets.size() - 1) % _widgets.size();
    
    size_t startIndex = _selecedIndx;
    while (_widgets[_selecedIndx] == nullptr || !_widgets[_selecedIndx]->isEnabled())
    {
        _selecedIndx = (_selecedIndx + _widgets.size() - 1) % _widgets.size();
        if (_selecedIndx == startIndex) return; // Avoid infinite loop if all widgets are null or hidden
    }
    
        _widgets[_selecedIndx]->focus();
}

void FocusManager::refresh() {
    if (_widgets.empty()) return;
    
    // if current valid, skip
    if (_selecedIndx < _widgets.size() && _widgets[_selecedIndx] && _widgets[_selecedIndx]->isEnabled()) {
        if (!_widgets[_selecedIndx]->isFocused()) {
            _widgets[_selecedIndx]->focus();
        }
        return;
    };

    if (_selecedIndx < _widgets.size() && _widgets[_selecedIndx]) _widgets[_selecedIndx]->unfocus();

    size_t startIndex = std::min(_selecedIndx, _widgets.size() - 1);

    while (_widgets[_selecedIndx] == nullptr || !_widgets[_selecedIndx]->isEnabled())
    {
        _selecedIndx = (_selecedIndx + _widgets.size() - 1) % _widgets.size();
        if (_selecedIndx == startIndex) return; // Avoid infinite loop if all widgets are null or hidden
    }

    _widgets[_selecedIndx]->focus();
}

} // namespace ui