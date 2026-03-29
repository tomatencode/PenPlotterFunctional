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

    uint8_t spacing = 0;
    uint8_t marginLeft = 0;
    uint8_t marginRight = 0;
    uint8_t marginTop = 0;
    uint8_t marginBottom = 0;
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
    struct Child {
        Widget* widget;
        uint16_t minPrimary;
        uint16_t finalPrimary;
        uint16_t secondary;
        bool expand;
        bool locked = false;
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

    uint16_t availablePrimary(Rect r) const;
    uint16_t availableSecondary(Rect r) const;

    Spacing computeSpacing(uint16_t available, uint16_t total, size_t count) const;
    void distributeExpansion(std::vector<Child>& children, uint16_t available) const;
    std::optional<Rect> computeChildRect(const Child& c, Rect content, int childStart) const;

    std::vector<LayoutItem> computeLayout(Rect content) const;

private:
    Axis _axis;
    LinearLayoutStyle _style;
};

} // namespace widgets
} // namespace ui