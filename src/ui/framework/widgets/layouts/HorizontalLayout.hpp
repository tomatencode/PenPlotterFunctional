#pragma once

#include <memory>
#include <vector>

#include "../core/LayoutWidget.hpp"

namespace ui {
namespace widgets {

class HorizontalLayout : public LayoutWidget
{
public:
    // Constructor: takes ownership of the provided widgets
    HorizontalLayout(std::vector<std::unique_ptr<Widget>>&& children, const LayoutStyle& style = LayoutStyle());

    void render(Renderer& r, Rect canvasBox) override;
    Size measure() const override;
    Size desiredSize(const Size& available) const override;

private:
    // Calculate total width of all children including spacing
    uint16_t getTotalWidth() const;
    
    // Calculate spacing for even distribution modes
    double getSpacing(uint16_t availableWidth) const;
};

} // namespace widgets
} // namespace ui
