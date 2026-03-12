#pragma once

#include "../core/Widget.hpp"

constexpr size_t MAX_LAYOUT_CHILDREN = 16;

class VerticalLayout : public Widget
{
public:
    // Constructor: just children, no position/size - parent provides canvas
    VerticalLayout(Widget* children[], size_t count);

    void render(Renderer& r, Rect canvasBox) override;
    Size measure() const override;

    size_t childCount() const override { return _count; }
    Widget* child(size_t index) const override;

private:
    Widget* _children[MAX_LAYOUT_CHILDREN]{nullptr};
    size_t _count{0};
    
    // Calculate total height of all children
    uint16_t getTotalHeight() const;
};
