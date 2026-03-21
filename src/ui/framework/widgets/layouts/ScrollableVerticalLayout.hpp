#pragma once

#include <memory>
#include <vector>

#include "VerticalLayout.hpp"
#include "LayoutStyle.hpp"

namespace ui {
namespace widgets {

struct ScrollableVerticalLayoutStyle {
    uint8_t spacing = 0;
    uint8_t marginLeft = 0;
    uint8_t marginRight = 0;
    uint8_t marginTop = 0;
    uint8_t marginBottom = 0;

    // Alignment - will be interpreted differently by horizontal vs vertical layouts
    HorizontalAlignment horizontalAlign = HorizontalAlignment::Left;

    constexpr ScrollableVerticalLayoutStyle() = default;

    constexpr ScrollableVerticalLayoutStyle(HorizontalAlignment hAlign, uint8_t space = 0,
                                            uint8_t mLeft = 0, uint8_t mRight = 0,
                                            uint8_t mTop = 0, uint8_t mBottom = 0)
        :   spacing(space),
            marginLeft(mLeft), marginRight(mRight),
            marginTop(mTop), marginBottom(mBottom),
            horizontalAlign(hAlign) {}
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
