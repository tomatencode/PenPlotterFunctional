#pragma once

#include "../core/LayoutWidget.hpp"

class VerticalLayout : public LayoutWidget
{
public:
    // Constructor: children with layout style
    VerticalLayout(Widget* children[], size_t count, const LayoutStyle& style = LayoutStyle());

    void render(Renderer& r, Rect canvasBox) override;
    Size measure() const override;

private:
    // Calculate spacing for even distribution modes
    uint8_t getSpacing(uint16_t availableHeight) const;
};
