#pragma once

#include <memory>
#include <vector>

#include "../core/LayoutWidget.hpp"
#include "LayoutStyle.hpp"

namespace ui {
namespace widgets {

struct HorizontalLayoutStyle {
    SpacingMode spacingMode = SpacingMode::Fixed;
    uint8_t spacing = 0;  // Used for Fixed mode
    uint8_t marginLeft = 0;
    uint8_t marginRight = 0;
    uint8_t marginTop = 0;
    uint8_t marginBottom = 0;

    // Alignment - will be interpreted differently by horizontal vs vertical layouts
    VerticalAlignment verticalAlign = VerticalAlignment::Top;

    constexpr HorizontalLayoutStyle() = default;

    constexpr HorizontalLayoutStyle(SpacingMode mode, VerticalAlignment vAlign, uint8_t space = 0,
                            uint8_t mLeft = 0, uint8_t mRight = 0,
                            uint8_t mTop = 0, uint8_t mBottom = 0)
        : spacingMode(mode), spacing(space),
            marginLeft(mLeft), marginRight(mRight),
            marginTop(mTop), marginBottom(mBottom),
            verticalAlign(vAlign) {}
};

class HorizontalLayout : public LayoutWidget
{
public:
    // Constructor: takes ownership of the provided widgets
    HorizontalLayout(const HorizontalLayoutStyle& style, std::vector<std::unique_ptr<Widget>>&& children);

    void render(Renderer& r, Rect canvasBox) override;
    Size measure() const override;
    Size desiredSize(const Size& available) const override;

private:
    // Calculate spacing for even distribution modes
    double getSpacing(uint16_t availableWidth) const;

    HorizontalLayoutStyle _style;
};

} // namespace widgets
} // namespace ui
