#pragma once

#include <memory>
#include <vector>

#include "../core/LayoutWidget.hpp"
#include "LayoutStyle.hpp"

namespace ui {
namespace widgets {

struct VerticalLayoutStyle {
    SpacingMode spacingMode; // default:  0 = SpacingMode::Fixed
    HorizontalAlignment horizontalAlign;  // default: 0 = HorizontalAlignment::Left
    uint8_t spacing;  // default: 0
    uint8_t marginLeft;  // default: 0
    uint8_t marginRight;  // default: 0
    uint8_t marginTop;  // default: 0
    uint8_t marginBottom;  // default: 0
};

class VerticalLayout : public LayoutWidget
{
public:
    // Constructor: takes ownership of the provided widgets
    VerticalLayout(const VerticalLayoutStyle& style, std::vector<std::unique_ptr<Widget>>&& children);

    void render(Renderer& r, Rect canvasBox) override;
    Size measure() const override;
    Size desiredSize(const Size& available) const override;

private:
    // Calculate spacing for even distribution modes
    double getSpacing(uint16_t availableHeight) const;

    VerticalLayoutStyle _style;
};

} // namespace widgets
} // namespace ui

