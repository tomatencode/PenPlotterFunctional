#pragma once

#include <memory>
#include <vector>

#include "../core/LayoutWidget.hpp"
#include "LayoutStyle.hpp"

namespace ui {
namespace widgets {

struct VerticalLayoutStyle {
    SpacingMode spacingMode = SpacingMode::Fixed;
    HorizontalAlignment horizontalAlign = HorizontalAlignment::Left;
    uint8_t spacing = 0;
    uint8_t marginLeft = 0;
    uint8_t marginRight = 0;
    uint8_t marginTop = 0;
    uint8_t marginBottom = 0;
};

class VerticalLayout : public LayoutWidget
{
public:
    // Constructor: takes ownership of the provided widgets
    VerticalLayout(const VerticalLayoutStyle& style, std::vector<std::unique_ptr<Widget>>&& children);

    void render(Renderer& r, Rect canvasBox) override;
    Size measure() const override;
    bool canExpandHorizontally() const override;
    bool canExpandVertically() const override;

private:
    // Calculate spacing for even distribution modes
    double getSpacing(uint16_t availableHeight) const;

    VerticalLayoutStyle _style;
};

} // namespace widgets
} // namespace ui

