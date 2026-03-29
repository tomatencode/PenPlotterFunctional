#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <cmath>
#include <optional>

#include "../core/Layout.hpp"
#include "LayoutStyle.hpp"

namespace ui {
namespace widgets {

struct LinearLayoutStyle {
    SpacingMode spacingMode = SpacingMode::Fixed;
    VerticalAlignment verticalAlign = VerticalAlignment::Top;
    HorizontalAlignment horizontalAlign = HorizontalAlignment::Left;

    uint16_t spacing = 0;
    uint16_t marginLeft = 0;
    uint16_t marginRight = 0;
    uint16_t marginTop = 0;
    uint16_t marginBottom = 0;
};

class LinearLayout : public Layout
{
public:
    LinearLayout(Axis axis,
                 const LinearLayoutStyle& style,
                 std::vector<std::unique_ptr<Widget>>&& children)
        : Layout(std::move(children)), _axis(axis), _style(style) {}

    template <typename... Children>
    LinearLayout(Axis axis, const LinearLayoutStyle& style, Children&&... children)
        : Layout(std::forward<Children>(children)...), _axis(axis), _style(style) {}

    void render(Renderer& r, Rect canvasBox) override;
    Size measure() const override;
    bool canExpandHorizontally() const override;
    bool canExpandVertically() const override;

private:
    struct ChildInfo {
        Widget* widget;
        uint16_t minPrimarySize;
        uint16_t finalPrimarySize;
        uint16_t secondarySize;
        bool canExpand;
        bool expansionLocked = false;
    };

    struct LayoutItem {
        Widget* widget;
        Rect rect;
    };

    struct Spacing {
        double between = 0;
        double leading = 0;
    };

private:
    // helpers
    Rect applyMargins(Rect box) const;

    uint16_t primarySize(Size s) const;
    uint16_t secondarySize(Size s) const;

    uint16_t PrimaryPos(Rect r) const;
    uint16_t SecondaryPos(Rect r) const;
    uint16_t availablePrimarySpace(Rect r) const;
    uint16_t availableSecondarySpace(Rect r) const;

    Spacing computeSpacing(uint16_t available, uint16_t total, size_t count) const;
    void distributeExpansion(std::vector<ChildInfo>& children, uint16_t available) const;
    std::optional<Rect> computeChildRect(const ChildInfo& c, Rect content, uint16_t childStart) const;

    std::vector<LayoutItem> computeLayout(Rect content) const;

private:
    Axis _axis;
    LinearLayoutStyle _style;
};

} // namespace widgets
} // namespace ui