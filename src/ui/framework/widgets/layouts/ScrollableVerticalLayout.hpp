#pragma once

#include <memory>
#include <vector>

#include "../core/Layout.hpp"
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

class ScrollableVerticalLayout : public Layout
{
public:
    // Vector-based constructor (for pre-built child vectors)
    ScrollableVerticalLayout(const ScrollableVerticalLayoutStyle& style, std::vector<std::unique_ptr<Widget>>&& children)
        : Layout(std::move(children)), _style(style)
    {
    }

    // Variadic constructor (for individual widget arguments)
    template <typename... Children>
    ScrollableVerticalLayout(const ScrollableVerticalLayoutStyle& style, Children&&... children)
        : Layout(std::forward<Children>(children)...), _style(style)
    {
    }

    void render(Renderer& r, Rect canvasBox) override;
    Size measure() const override;
    bool canExpandHorizontally() const override;
    bool canExpandVertically() const override;

private:
    int16_t _scrollOffset = 0;

    // Find which child is focused and update scroll offset to ensure it's visible
    void updateScrollOffset(uint8_t visibleHeight);

    ScrollableVerticalLayoutStyle _style;
};

} // namespace widgets
} // namespace ui
