#pragma once

#include "VerticalLayout.hpp"

class ScrollableVerticalLayout : public Widget
{
public:
    // Constructor: just children, parent provides canvas
    ScrollableVerticalLayout(Widget* children[], size_t count, const LayoutStyle& style = LayoutStyle());

    void render(Renderer& r, Rect canvasBox) override;
    Size measure() const override;

    size_t childCount() const override { return _count; }
    Widget* child(size_t index) const override;

private:
    Widget* _children[MAX_LAYOUT_CHILDREN]{nullptr};
    size_t _count{0};

    const LayoutStyle _style;

    int16_t _scrollOffset = 0;

    // Find which child is focused and update scroll offset
    void updateScrollOffset(uint8_t visibleHeight);
};