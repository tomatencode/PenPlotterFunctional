#include "ProgressBar.hpp"

namespace ui {
namespace widgets {

ProgressBar::ProgressBar(ProgressBarStyle style, std::function<double()> getProgress)
    : Widget(),
      _style(style),
      _getProgress(getProgress)
{}

Size ProgressBar::measure() const
{
    return Size{2, 1}; // Minimum size to show empty bar: "[]"
}

void ProgressBar::render(Renderer& r, Rect canvasBox)
{
    if (canvasBox.w == 0 || canvasBox.h == 0)
        return; // nothing visible

    uint16_t x = canvasBox.x;
    uint16_t y = canvasBox.y;
    uint16_t barWidth = canvasBox.w;
    uint16_t barHeight = canvasBox.h;

    // Calculate inner bar width (excluding brackets)
    uint16_t innerWidth = (barWidth > 2) ? (barWidth - 2) : 0;
    double progress = (_getProgress) ? _getProgress() : 0;
    progress = std::min(std::max(progress, 0.0), 1.0); // Clamp to [0, 1]
    uint16_t filledWidth = (innerWidth * progress);

    // Draw opening bracket
    r.drawGlyphToBuffer(x, y, _style.leftBracket);

    // Draw filled and empty portions
    for (uint16_t col = 0; col < innerWidth; col++)
    {
        r.drawGlyphToBuffer(
            x + 1 + col,
            y,
            (col < filledWidth) ? _style.filledGlyph : _style.emptyGlyph
        );
    }

    // Draw closing bracket
    r.drawGlyphToBuffer(x + barWidth - 1, y, _style.rightBracket);
}

} // namespace widgets
} // namespace ui