#pragma once

#include <memory>
#include <vector>

#include "../core/LayoutWidget.hpp"
#include "LayoutStyle.hpp"

namespace ui {
namespace widgets {

struct HorizontalLayoutStyle {
    SpacingMode spacingMode; // default:  0 = SpacingMode::Fixed
    VerticalAlignment verticalAlign; // default: 0 = VerticalAlignment::Top
    uint8_t spacing; // default: 0
    uint8_t marginLeft; // default: 0
    uint8_t marginRight; // default: 0
    uint8_t marginTop; // default: 0
    uint8_t marginBottom; // default: 0
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
