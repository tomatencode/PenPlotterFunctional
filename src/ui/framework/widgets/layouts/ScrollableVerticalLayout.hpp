#pragma once

#include <memory>
#include <vector>
#include <algorithm>

#include "../core/Layout.hpp"
#include "LayoutStyle.hpp"

namespace ui {
namespace widgets {

struct ScrollableVerticalLayoutStyle {
    HorizontalAlignment horizontalAlign = HorizontalAlignment::Left;
    uint16_t spacing = 0;
    uint16_t marginLeft = 0;
    uint16_t marginRight = 0;
    uint16_t marginTop = 0;
    uint16_t marginBottom = 0;
};

class ScrollableVerticalLayout : public Layout
{
public:
    ScrollableVerticalLayout(const ScrollableVerticalLayoutStyle& style,
                             std::vector<std::unique_ptr<Widget>>&& children)
        : Layout(std::move(children)), _style(style)
    {}

    template <typename... Children>
    ScrollableVerticalLayout(const ScrollableVerticalLayoutStyle& style, Children&&... children)
        : Layout(std::forward<Children>(children)...), _style(style)
    {}

    void render(Renderer& r, Box canvasBox) override;
    Size measure() const override;
    bool canExpandHorizontally() const override;
    bool canExpandVertically() const override;

private:
    struct ChildInfo {
        Widget* widget;
        uint16_t width;   // clamped to content width
        uint16_t height;
    };

    struct LayoutItem {
        Widget* widget;
        Box rect;
    };

private:
    Box applyMargins(Box box) const;
    std::vector<ChildInfo> collectChildren(Box content) const;
    std::vector<LayoutItem> computeLayout(Box content);
    bool containsFocusedWidget(Widget* widget) const;

private:
    uint16_t _scrollOffset = 0;
    ScrollableVerticalLayoutStyle _style;
};

} // namespace widgets
} // namespace ui