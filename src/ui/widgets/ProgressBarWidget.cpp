#include "ProgressBarWidget.hpp"
#include "../widgetSystem/WidgetUtils.hpp"

ProgressBarWidget::ProgressBarWidget(Rect box,
                                     uint8_t (*getProgress)(),
                                     Alignment align)
    : Widget(box, align),
      _getProgress(getProgress)
{}

Size ProgressBarWidget::measure() const
{
    return Size{box().w, box().h};
}

void ProgressBarWidget::render(Renderer& r, Rect canvasBox)
{
    // Compute final drawing rectangle including alignment + clipping
    Rect drawRect = computeContentAlignment(box(), align(), measure(), canvasBox);

    if (drawRect.w == 0 || drawRect.h == 0)
        return; // nothing visible

    int x = drawRect.x;
    int y = drawRect.y;
    uint8_t barWidth = drawRect.w;
    uint8_t barHeight = drawRect.h;

    // Calculate inner bar width (excluding brackets)
    uint8_t innerWidth = (barWidth > 2) ? (barWidth - 2) : 0;
    uint8_t progress = (_getProgress != nullptr) ? _getProgress() : 0;
    progress = (progress > 100) ? 100 : progress;
    uint8_t filledWidth = (innerWidth * progress) / 100;

    // Draw the progress bar
    for (uint8_t row = 0; row < barHeight; row++)
    {
        // Draw opening bracket
        r.drawGlyphToBuffer(x, y + row, Glyph{'['});

        // Draw filled and empty portions
        for (uint8_t col = 0; col < innerWidth; col++)
        {
            Glyph g = (col < filledWidth) ? Glyph{'='} : Glyph{' '};
            r.drawGlyphToBuffer(x + 1 + col, y + row, g);
        }

        // Draw closing bracket
        r.drawGlyphToBuffer(x + barWidth - 1, y + row, Glyph{']'});
    }
}
