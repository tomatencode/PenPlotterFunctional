#pragma once

#include "../core/Widget.hpp"
#include "../core/LayoutStyle.hpp"
#include "config/ui_config.hpp"

class HorizontalLayout : public Widget
{
public:
    // Constructor: children with layout style
    HorizontalLayout(Widget* children[], size_t count, const LayoutStyle& style = LayoutStyle());

    void render(Renderer& r, Rect canvasBox) override;
    Size measure() const override;

    size_t childCount() const override { return _count; }
    Widget* child(size_t index) const override;

private:
    Widget* _children[MAX_LAYOUT_CHILDREN]{nullptr};
    size_t _count{0};
    LayoutStyle _style;
    
    // Calculate total width of all children including spacing
    uint16_t getTotalWidth() const;
    
    // Calculate spacing for even distribution modes
    uint8_t getSpacing(uint16_t availableWidth) const;
};