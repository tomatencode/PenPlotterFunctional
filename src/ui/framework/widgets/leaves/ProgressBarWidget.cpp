#include "ProgressBarWidget.hpp"

namespace ui {
namespace widgets {

ProgressBarWidget::ProgressBarWidget(ProgressBarStyle style, std::function<uint8_t()> getProgress)
    : Widget(),
      _style(style),
      _getProgress(getProgress)
{}

Size ProgressBarWidget::measure() const
{
    return Size{2, 1}; // Minimum size to show empty bar: "[]"
}

Size ProgressBarWidget::desiredSize(const Size& available) const
{
    return Size{available.w, 1}; // Fill available width, but only 1 row high
}

void ProgressBarWidget::render(Renderer& r, Rect canvasBox)
{
    if (canvasBox.w == 0 || canvasBox.h == 0)
        return; // nothing visible

    int x = canvasBox.x;
    int y = canvasBox.y;
    uint8_t barWidth = canvasBox.w;
    uint8_t barHeight = canvasBox.h;

    // Calculate inner bar width (excluding brackets)
    uint8_t innerWidth = (barWidth > 2) ? (barWidth - 2) : 0;
    uint8_t progress = (_getProgress != nullptr) ? _getProgress() : 0;
    progress = (progress > 100) ? 100 : progress;
    uint8_t filledWidth = (innerWidth * progress) / 100;

    // Draw the progress bar
    for (uint8_t row = 0; row < barHeight; row++)
    {
        // Draw opening bracket
        r.drawGlyphToBuffer(x, y + row, _style.leftBracket);

        // Draw filled and empty portions
        for (uint8_t col = 0; col < innerWidth; col++)
        {
            Glyph g = (col < filledWidth) ? _style.filledGlyph : _style.emptyGlyph;
            r.drawGlyphToBuffer(x + 1 + col, y + row, g);
        }

        // Draw closing bracket
        r.drawGlyphToBuffer(x + barWidth - 1, y + row, _style.rightBracket);
    }
}

} // namespace widgets
} // namespace ui