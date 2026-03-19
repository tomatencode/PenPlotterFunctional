#pragma once

#include <memory>
#include <vector>

#include "VerticalLayout.hpp"

namespace ui {
namespace widgets {

class ScrollableVerticalLayout : public LayoutWidget
{
public:
    // Constructor: takes ownership of the provided widgets
    ScrollableVerticalLayout(std::vector<std::unique_ptr<Widget>>&& children, const LayoutStyle& style = LayoutStyle());

    void render(Renderer& r, Rect canvasBox) override;
    Size measure() const override;
    Size desiredSize(const Size& available) const override;

private:
    int16_t _scrollOffset = 0;

    // Find which child is focused and update scroll offset
    void updateScrollOffset(uint8_t visibleHeight);
};

} // namespace widgets
} // namespace ui
