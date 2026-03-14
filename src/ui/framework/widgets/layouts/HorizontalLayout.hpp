#pragma once

#include "../core/LayoutWidget.hpp"

class HorizontalLayout : public LayoutWidget
{
public:
    // Constructor: children with layout style
    HorizontalLayout(Widget* children[], size_t count, const LayoutStyle& style = LayoutStyle());

    void render(Renderer& r, Rect canvasBox) override;
    Size measure() const override;

private:
    // Calculate total width of all children including spacing
    uint16_t getTotalWidth() const;
    
    // Calculate spacing for even distribution modes
    uint8_t getSpacing(uint16_t availableWidth) const;
};