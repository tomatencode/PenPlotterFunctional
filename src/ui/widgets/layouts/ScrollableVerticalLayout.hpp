#pragma once

#include "VerticalLayout.hpp"

class ScrollableVerticalLayout : public VerticalLayout
{
public:
    // Constructor: just children, parent provides canvas
    ScrollableVerticalLayout(Widget* children[], size_t count);

    void render(Renderer& r, Rect canvasBox) override;

private:
    int16_t _scrollOffset = 0;

    // Find which child is focused and update scroll offset
    void updateScrollOffset(uint8_t visibleHeight);
};