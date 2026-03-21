#pragma once

#include <memory>
#include <vector>

#include "VerticalLayout.hpp"
#include "LayoutStyle.hpp"

namespace ui {
namespace widgets {

struct ScrollableVerticalLayoutStyle {
    HorizontalAlignment horizontalAlign = HorizontalAlignment::Left;
    uint8_t spacing = 0;
    uint8_t marginLeft = 0;
    uint8_t marginRight = 0;
    uint8_t marginTop = 0;
    uint8_t marginBottom = 0;
};

class ScrollableVerticalLayout : public LayoutWidget
{
public:
    // Constructor: takes ownership of the provided widgets
    ScrollableVerticalLayout(const ScrollableVerticalLayoutStyle& style, std::vector<std::unique_ptr<Widget>>&& children);

    void render(Renderer& r, Rect canvasBox) override;
    Size measure() const override;
    Size desiredSize(const Size& available) const override;

private:
    int16_t _scrollOffset = 0;

    // Find which child is focused and update scroll offset
    void updateScrollOffset(uint8_t visibleHeight);

    ScrollableVerticalLayoutStyle _style;
};

} // namespace widgets
} // namespace ui
