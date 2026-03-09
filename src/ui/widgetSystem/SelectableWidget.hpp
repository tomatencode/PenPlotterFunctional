#ifndef SELECTABLE_WIDGET_HPP
#define SELECTABLE_WIDGET_HPP

#include "Widget.hpp"
#include "../InputState.hpp"

class SelectableWidget : public Widget
{
public:
    SelectableWidget(Rect box) : Widget(box) {}

    virtual WidgetType type() const override { return WidgetType::Selectable; }

    virtual void focus() { _focused = true; onFocusGained(); }
    virtual void unfocus() { _focused = false; onFocusLost(); }
    virtual bool isFocused() const { return _focused; }

    virtual void handleInput(InputState& input) = 0;

private:
    bool _focused = false;

    virtual void onFocusGained() {}
    virtual void onFocusLost() {}
};

#endif