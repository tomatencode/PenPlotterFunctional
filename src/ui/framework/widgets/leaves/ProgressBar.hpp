#pragma once

#include <functional>

#include "../core/Widget.hpp"

namespace ui {
namespace widgets {

struct ProgressBarStyle
{
    Glyph emptyGlyph   = ' ';
    Glyph filledGlyph  = '=';
    Glyph leftBracket  = '[';
    Glyph rightBracket = ']';
};

struct ProgressBarProps
{
    ProgressBarStyle style = {};
    std::function<double()> getProgress = nullptr;
};

class ProgressBar : public Widget
{
public:
    ProgressBar(ProgressBarProps props);

    void render(Renderer& r, Box canvasBox) override;
    Size measure() const override;
    bool canExpandHorizontally() const override { return true; }

private:
    ProgressBarProps _props;
};

} // namespace widgets
} // namespace ui