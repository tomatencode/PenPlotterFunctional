#pragma once

#include "VerticalLayout.hpp"

class ScrollableVerticalLayout : public LayoutWidget
{
public:
    // Constructor: just children, parent provides canvas
    ScrollableVerticalLayout(Widget* children[], size_t count, const LayoutStyle& style = LayoutStyle());

    void render(Renderer& r, Rect canvasBox) override;
    Size measure() const override;

private:
    int16_t _scrollOffset = 0;

    // Find which child is focused and update scroll offset
    void updateScrollOffset(uint8_t visibleHeight);
};