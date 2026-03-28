#pragma once

#include <memory>
#include <vector>
#include <algorithm>

#include "../core/Layout.hpp"
#include "LayoutStyle.hpp"

namespace ui {
namespace widgets {

struct HorizontalLayoutStyle {
    SpacingMode spacingMode = SpacingMode::Fixed;
    VerticalAlignment verticalAlign = VerticalAlignment::Top;
    uint8_t spacing = 0;
    uint8_t marginLeft = 0;
    uint8_t marginRight = 0;
    uint8_t marginTop = 0;
    uint8_t marginBottom = 0;
};

class HorizontalLayout : public Layout
{
public:
    HorizontalLayout(const HorizontalLayoutStyle& style, std::vector<std::unique_ptr<Widget>>&& children)
        : Layout(std::move(children)), _style(style)
    {}

    template <typename... Children>
    HorizontalLayout(const HorizontalLayoutStyle& style, Children&&... children)
        : Layout(std::forward<Children>(children)...), _style(style)
    {}

    void render(Renderer& r, Rect canvasBox) override;
    Size measure() const override;
    bool canExpandHorizontally() const override;
    bool canExpandVertically() const override;

private:
    struct LayoutItem {
        Widget* widget;
        Rect rect;
    };

    struct Metrics {
        uint16_t totalWidth = 0;
        uint16_t maxHeight = 0;
    };

    Rect applyMargins(Rect box) const;
    Metrics computeMetrics() const;
    double computeSpacing(uint16_t availableWidth, uint16_t totalChildWidth, size_t count) const;
    std::vector<LayoutItem> computeLayout(Rect contentArea) const;

    HorizontalLayoutStyle _style;
};

} // namespace widgets
} // namespace ui