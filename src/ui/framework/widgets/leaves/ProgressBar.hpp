#pragma once

#include <functional>

#include "../core/Widget.hpp"

namespace ui {
namespace widgets {

struct ProgressBarStyle
{
    Glyph emptyGlyph = ' ';
    Glyph filledGlyph = '=';
    Glyph leftBracket = '[';
    Glyph rightBracket = ']';
};

class ProgressBar : public Widget
{
public:
    // Construct a progress bar with specified width and height
    // getProgress: function pointer that returns progress 0-100
    ProgressBar(ProgressBarStyle style, std::function<double()> getProgress);

    void render(Renderer& r, Rect canvasBox) override;
    Size measure() const override;
    bool canExpandHorizontally() const override { return true; }

private:
    ProgressBarStyle _style;
    std::function<double()> _getProgress;  // Function object returning 0-1
};

} // namespace widgets
} // namespace ui