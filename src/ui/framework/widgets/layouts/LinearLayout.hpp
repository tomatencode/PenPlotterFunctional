#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <cmath>

#include "../core/Layout.hpp"
#include "LayoutStyle.hpp"

namespace ui {
namespace widgets {

struct LinearLayoutStyle {
    SpacingMode spacingMode = SpacingMode::Fixed;
    VerticalAlignment verticalAlign = VerticalAlignment::Top;
    HorizontalAlignment horizontalAlign = HorizontalAlignment::Left;

    uint8_t spacing = 0;
    uint8_t marginLeft = 0;
    uint8_t marginRight = 0;
    uint8_t marginTop = 0;
    uint8_t marginBottom = 0;
};

class LinearLayout : public Layout
{
public:
    LinearLayout(const Axis primaryAxis,
                 const LinearLayoutStyle& style,
                 std::vector<std::unique_ptr<Widget>>&& children
                )
        : Layout(std::move(children)), _style(style), _primaryAxis(primaryAxis)
    {}

    template <typename... Children>
    LinearLayout(const Axis primaryAxis, const LinearLayoutStyle& style, Children&&... children)
        : Layout(std::forward<Children>(children)...), _style(style), _primaryAxis(primaryAxis)
    {}

    void render(Renderer& r, Rect canvasBox) override;
    Size measure() const override;
    bool canExpandHorizontally() const override;
    bool canExpandVertically() const override;

private:
    struct ChildInfo {
        Widget* widget;
        uint16_t minWidth;
        uint16_t width;
        uint16_t height;
        bool expand;
        bool locked = false;
    };

    struct LayoutItem {
        Widget* widget;
        Rect rect;
    };

    struct SpacingInfo {
        double between;
        double around;
    };

private:

    Rect applyMargins(Rect box) const;

    SpacingInfo computeSpacing(uint16_t availableSpace,
                               uint16_t totalChildSize,
                               size_t count) const;

    std::vector<LayoutItem> computeLayout(Rect contentArea) const;

    std::vector<ChildInfo> expandExpandableChildren(std::vector<ChildInfo> children,
                                                    uint16_t availableSpace) const;

private:
    Axis _primaryAxis;
    LinearLayoutStyle _style;
};

} // namespace widgets
} // namespace ui