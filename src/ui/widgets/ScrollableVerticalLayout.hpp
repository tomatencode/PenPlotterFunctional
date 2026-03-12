#pragma once

#include "VerticalLayout.hpp"

class ScrollableVerticalLayout : public VerticalLayout
{
public:
    // Constructor: just children, parent provides canvas
    ScrollableVerticalLayout(Widget* children[], size_t count);

    void render(Renderer& r, Rect canvasBox) override;

    // Call this when focus changes to ensure focused child is visible
    void ensureFocusedChildVisible();

private:
    int16_t _scrollOffset = 0;

    // Find which child is focused and update scroll offset
    void updateScrollOffset();

    // Get the Y position of a child considering scroll offset
    int16_t getChildY(size_t index) const;
};