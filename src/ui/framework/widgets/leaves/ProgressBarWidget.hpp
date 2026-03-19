#pragma once

#include <functional>

#include "../core/Widget.hpp"

namespace ui {
namespace widgets {

class ProgressBarWidget : public Widget
{
public:
    // Construct a progress bar with specified width and height
    // getProgress: function pointer that returns progress 0-100
    ProgressBarWidget(std::function<uint8_t()> getProgress);

    void render(Renderer& r, Rect canvasBox) override;
    Size measure() const override;
    Size desiredSize(const Size& available) const override;

private:
    std::function<uint8_t()> _getProgress;  // Function object returning 0-100
};

} // namespace widgets
} // namespace ui