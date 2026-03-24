#pragma once

#include "Widget.hpp"
#include "../../input/InputState.hpp"

namespace ui {
namespace widgets {

class Selectable : public Widget
{
public:
    Selectable() {}
    
    ~Selectable() = default;

    virtual bool isSelectable() const override { return true; }

    virtual void focus() { _focused = true; onFocusGained(); }
    virtual void unfocus() { _focused = false; onFocusLost(); }
    virtual bool isFocused() const { return _focused; }

    virtual void handleInput(InputState& input) = 0;

    virtual void reload() override {
        _focused = false;
        onFocusLost();
    }

private:
    bool _focused = false;

    virtual void onFocusGained() {}
    virtual void onFocusLost() {}
};

} // namespace widgets
} // namespace ui
