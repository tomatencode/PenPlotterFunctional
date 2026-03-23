#pragma once

#include <memory>
#include <vector>

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
    // Vector-based constructor (for pre-built child vectors)
    HorizontalLayout(const HorizontalLayoutStyle& style, std::vector<std::unique_ptr<Widget>>&& children)
        : Layout(std::move(children)), _style(style)
    {
    }

    // Variadic constructor (for individual widget arguments)
    template <typename... Children>
    HorizontalLayout(const HorizontalLayoutStyle& style, Children&&... children)
        : Layout(std::forward<Children>(children)...), _style(style)
    {
    }

    void render(Renderer& r, Rect canvasBox) override;
    Size measure() const override;
    bool canExpandHorizontally() const override;
    bool canExpandVertically() const override;

private:
    // Calculate spacing for even distribution modes
    double getSpacing(uint16_t availableWidth) const;

    HorizontalLayoutStyle _style;
};

} // namespace widgets
} // namespace ui
