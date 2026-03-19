#pragma once

#include <memory>
#include <vector>

#include "../core/LayoutWidget.hpp"

namespace ui {
namespace widgets {

class VerticalLayout : public LayoutWidget
{
public:
    // Constructor: takes ownership of the provided widgets
    VerticalLayout(std::vector<std::unique_ptr<Widget>>&& children, const LayoutStyle& style = LayoutStyle());

    void render(Renderer& r, Rect canvasBox) override;
    Size measure() const override;

private:
    // Calculate spacing for even distribution modes
    double getSpacing(uint16_t availableHeight) const;
};

} // namespace widgets
} // namespace ui

