#pragma once

#include <cstddef>
#include <vector>
#include "../widgets/core/Widget.hpp"
#include "../widgets/core/ISelectable.hpp"

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
    void refresh(std::vector<widgets::ISelectable*> selectableWidgets);

    void next();
    void prev();

    size_t _focusedIndx = 0;
    widgets::Widget* _root = nullptr;
    std::vector<widgets::ISelectable*> _cachedSelectables;

    void collectSelectables(widgets::Widget* w, std::vector<widgets::ISelectable*>& out);
};

} // namespace ui