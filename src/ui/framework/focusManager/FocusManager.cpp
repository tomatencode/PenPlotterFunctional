#include "FocusManager.hpp"

namespace ui {

FocusManager::FocusManager(widgets::Widget* root, size_t initialFocusIndex)
    : _root(root),
    _focusedIndx(initialFocusIndex)
{}

void FocusManager::setRoot(widgets::Widget* root, size_t initialFocusIndex) {
    _root = root;
    _focusedIndx = initialFocusIndex;
}

void FocusManager::handleInput(InputState& input)
{
    std::vector<widgets::Selectable*> selectableWidgets;
    collectSelectables(_root, selectableWidgets);

    if (selectableWidgets.empty() || selectableWidgets[_focusedIndx] == nullptr) return;

    selectableWidgets[_focusedIndx]->handleInput(input);

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
    std::vector<widgets::Selectable*> selectableWidgets;
    collectSelectables(_root, selectableWidgets);

    if (selectableWidgets.empty()) return;

    if (selectableWidgets[_focusedIndx] != nullptr)
        selectableWidgets[_focusedIndx]->unfocus();
    
    _focusedIndx = (_focusedIndx + 1) % selectableWidgets.size();

    size_t startIndex = _focusedIndx;
    while (selectableWidgets[_focusedIndx] == nullptr || !selectableWidgets[_focusedIndx]->isEnabled())
    {
        _focusedIndx = (_focusedIndx + 1) % selectableWidgets.size();
        if (_focusedIndx == startIndex) return; // Avoid infinite loop if all selectableWidgets are null or hidden
    }
    
    selectableWidgets[_focusedIndx]->focus();
}

// Move focus to the previous widget
void FocusManager::prev()
{
    std::vector<widgets::Selectable*> selectableWidgets;
    collectSelectables(_root, selectableWidgets);

    if (selectableWidgets.empty()) return;

    if (selectableWidgets[_focusedIndx] != nullptr)
        selectableWidgets[_focusedIndx]->unfocus();

    _focusedIndx = (_focusedIndx + selectableWidgets.size() - 1) % selectableWidgets.size();
    
    size_t startIndex = _focusedIndx;
    while (selectableWidgets[_focusedIndx] == nullptr || !selectableWidgets[_focusedIndx]->isEnabled())
    {
        _focusedIndx = (_focusedIndx + selectableWidgets.size() - 1) % selectableWidgets.size();
        if (_focusedIndx == startIndex) return; // Avoid infinite loop if all selectableWidgets are null or hidden
    }
    
        selectableWidgets[_focusedIndx]->focus();
}

void FocusManager::refresh() {

    std::vector<widgets::Selectable*> selectableWidgets;
    collectSelectables(_root, selectableWidgets);

    if (selectableWidgets.empty()) return;
    
    // if current valid, skip
    if (_focusedIndx < selectableWidgets.size() && selectableWidgets[_focusedIndx] && selectableWidgets[_focusedIndx]->isEnabled()) {
        if (!selectableWidgets[_focusedIndx]->isFocused()) {
            selectableWidgets[_focusedIndx]->focus();
        }
        return;
    };

    if (_focusedIndx < selectableWidgets.size() && selectableWidgets[_focusedIndx]) selectableWidgets[_focusedIndx]->unfocus();

    size_t startIndex = std::min(_focusedIndx, selectableWidgets.size() - 1);

    while (selectableWidgets[_focusedIndx] == nullptr || !selectableWidgets[_focusedIndx]->isEnabled())
    {
        _focusedIndx = (_focusedIndx + selectableWidgets.size() - 1) % selectableWidgets.size();
        if (_focusedIndx == startIndex) return; // Avoid infinite loop if all widgets are null or hidden
    }

    selectableWidgets[_focusedIndx]->focus();
}

void FocusManager::collectSelectables(widgets::Widget* w, std::vector<widgets::Selectable*>& out) {
    if (!w) return;

    if (w->isSelectable())
        out.push_back(static_cast<widgets::Selectable*>(w));

    for (size_t i = 0; i < w->childCount(); i++)
        collectSelectables(w->child(i), out);
}

} // namespace ui