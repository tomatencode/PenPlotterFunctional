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
    Axis axis           = Axis::Vertical;
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
    LinearLayout(const LinearLayoutStyle& style,
                 std::vector<std::unique_ptr<Widget>>&& children)
        : Layout(std::move(children)), _style(style) {}

    template <typename... Children>
    LinearLayout(const LinearLayoutStyle& style, Children&&... children)
        : Layout(std::forward<Children>(children)...), _style(style) {}

    void render(Renderer& r, Box canvasBox) override;
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
        Box rect;
    };

    struct Spacing {
        double between = 0;
        double leading = 0;
    };

private:
    // helpers
    Box applyMargins(Box box) const;

    uint16_t primarySize(Size s) const;
    uint16_t secondarySize(Size s) const;

    int16_t PrimaryPos(Box r) const;
    int16_t SecondaryPos(Box r) const;
    uint16_t availablePrimarySpace(Box r) const;
    uint16_t availableSecondarySpace(Box r) const;

    Spacing computeSpacing(uint16_t available, uint16_t total, size_t count) const;
    void distributeExpansion(std::vector<ChildInfo>& children, uint16_t available) const;
    std::optional<Box> computeChildRect(const ChildInfo& c, Box content, int16_t childStart) const;

    std::vector<LayoutItem> computeLayout(Box content) const;

private:
    LinearLayoutStyle _style;
};

} // namespace widgets
} // namespace ui