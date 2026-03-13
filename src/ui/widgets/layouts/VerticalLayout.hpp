#pragma once

#include "../core/Widget.hpp"
#include "../core/LayoutStyle.hpp"
#include "config/ui_config.hpp"

class VerticalLayout : public Widget
{
public:
    // Constructor: children with layout style
    VerticalLayout(Widget* children[], size_t count, const LayoutStyle& style = LayoutStyle());

    void render(Renderer& r, Rect canvasBox) override;
    Size measure() const override;

    size_t childCount() const override { return _count; }
    Widget* child(size_t index) const override;

private:
    Widget* _children[MAX_LAYOUT_CHILDREN]{nullptr};
    size_t _count{0};

    const LayoutStyle _style;
    
    // Calculate total height of all children including spacing
    uint16_t getTotalHeight() const;
    
    // Calculate spacing for even distribution modes
    uint8_t getSpacing(uint16_t availableHeight) const;
};
