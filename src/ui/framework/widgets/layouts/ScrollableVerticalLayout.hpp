#pragma once

#include <memory>
#include <vector>

#include "VerticalLayout.hpp"
#include "LayoutStyle.hpp"

namespace ui {
namespace widgets {

struct ScrollableVerticalLayoutStyle {
    HorizontalAlignment horizontalAlign;  // default: 0 = HorizontalAlignment::Left
    uint8_t spacing;  // default: 0
    uint8_t marginLeft;  // default: 0
    uint8_t marginRight;  // default: 0
    uint8_t marginTop;  // default: 0
    uint8_t marginBottom;  // default: 0
};

class ScrollableVerticalLayout : public LayoutWidget
{
public:
    // Constructor: takes ownership of the provided widgets
    ScrollableVerticalLayout(std::vector<std::unique_ptr<Widget>>&& children, const ScrollableVerticalLayoutStyle& style = ScrollableVerticalLayoutStyle());

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
