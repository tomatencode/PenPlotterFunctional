#include "FocusManager.hpp"

namespace ui {

FocusManager::FocusManager(widgets::Widget* root, size_t initialFocusIndex)
    : _root(root), _focusedIndx(initialFocusIndex)
{
    if (_root)
        refresh();
}

void FocusManager::setRoot(widgets::Widget* root, size_t initialFocusIndex) {
    _root = root;
    _focusedIndx = initialFocusIndex;

    if (_root)
        refresh();
}

void FocusManager::handleInput(InputState& input) {
    refresh();

    if (_cachedSelectables.empty()) return;

    // clamp index
    if (_focusedIndx >= _cachedSelectables.size())
        _focusedIndx = 0;

    auto* current = _cachedSelectables[_focusedIndx];
    if (!current) return;

    current->handleInput(input);

    // navigation widget input
    if (input.encoderDelta > 0)
        next();
    else if (input.encoderDelta < 0)
        prev();

    input.encoderDelta = 0;
}

void FocusManager::next() {
    if (_cachedSelectables.empty()) return;

    _cachedSelectables[_focusedIndx]->unfocus();

    _focusedIndx = (_focusedIndx + 1) % _cachedSelectables.size();

    _cachedSelectables[_focusedIndx]->focus();
}

void FocusManager::prev() {
    if (_cachedSelectables.empty()) return;

    _cachedSelectables[_focusedIndx]->unfocus();

    _focusedIndx = (_focusedIndx + _cachedSelectables.size() - 1) % _cachedSelectables.size();

    _cachedSelectables[_focusedIndx]->focus();
}

void FocusManager::refresh() {
    std::vector<widgets::ISelectable*> widgets;
    collectSelectables(_root, widgets);

    _cachedSelectables = widgets;

    if (widgets.empty()) return;

    // clamp index
    if (_focusedIndx >= widgets.size())
        _focusedIndx = widgets.size() - 1;

    auto* current = widgets[_focusedIndx];
    if (!current) return;

    if (!current->isFocused())
        current->focus();
}

void FocusManager::collectSelectables(widgets::Widget* w, std::vector<widgets::ISelectable*>& out) {
    if (!w) return;

    if (auto* sel = w->tryGetSelectable())
        out.push_back(sel);

    for (size_t i = 0; i < w->getChildCount(); i++)
        collectSelectables(w->getChild(i), out);
}

} // namespace ui