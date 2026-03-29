#pragma once

#include <cstddef>
#include <vector>
#include "../widgets/core/Selectable.hpp"

namespace ui {

class FocusManager
{
public:
    FocusManager(widgets::Widget* root, size_t initialFocusIndex = 0);

    void setRoot(widgets::Widget* root, size_t initialFocusIndex = 0);

    // Forward input to the currently focused widget
    void handleInput(InputState& input);

    void refresh();
private:
    void refresh(std::vector<widgets::Selectable*> selectableWidgets);

    void next();
    void prev();

    size_t _focusedIndx = 0;
    widgets::Widget* _root = nullptr;

    void collectSelectables(widgets::Widget* w, std::vector<widgets::Selectable*>& out);
};

} // namespace ui