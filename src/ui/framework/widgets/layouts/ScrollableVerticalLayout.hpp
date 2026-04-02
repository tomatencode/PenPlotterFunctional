#pragma once

#include <memory>
#include <vector>

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

    void render(Renderer& r, Rect canvasBox) override;
    Size measure() const override;
    bool canExpandHorizontally() const override;
    bool canExpandVertically() const override;

private:
    struct ChildInfo {
        Widget* widget;
        Size size;
    };

private:
    Rect applyMargins(Rect box) const;

    std::vector<ChildInfo> collectChildren() const;

    void updateScrollOffset(const std::vector<ChildInfo>& children,
                            uint16_t visibleHeight);

    bool containsFocusedWidget(Widget* widget) const;

private:
    int16_t _scrollOffset = 0;
    ScrollableVerticalLayoutStyle _style;
};

} // namespace widgets
} // namespace ui