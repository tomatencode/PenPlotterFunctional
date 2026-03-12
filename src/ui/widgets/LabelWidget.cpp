#include "LabelWidget.hpp"
#include "../render/Render.hpp"
#include "../widgetSystem/WidgetUtils.hpp"

LabelWidget::LabelWidget(TextSource& textSource)
    : text(textSource)
{}

Size LabelWidget::measure() const
{
    const Glyph* glyphs = text.getGlyphs();
    int width = 0;

    // Count glyphs until terminator or max LCD width
    while (glyphs[width].code != GLYPH_TERMINATOR.code && width < LCD_COLS)
        width++;

    return {
        static_cast<uint8_t>(width),
        static_cast<uint8_t>(1) // single line height
    };
}

void LabelWidget::render(Renderer& r, Rect canvasBox)
{
    if (canvasBox.w == 0 || canvasBox.h == 0)
        return;

    const Glyph* srcGlyphs = text.getGlyphs();

    int maxWidth = canvasBox.w;

    int textWidth = 0;
    while (srcGlyphs[textWidth].code != GLYPH_TERMINATOR.code && textWidth < maxWidth)
        textWidth++;

    if (textWidth == 0)
        return;

    int renderTextWidth = std::min(textWidth, maxWidth);

    Glyph renderGlyphs[LCD_COLS + 1];

    for (int i = 0; i < renderTextWidth; i++)
        renderGlyphs[i] = srcGlyphs[i];

    // truncate indicator
    if (textWidth > maxWidth && renderTextWidth > 0)
        renderGlyphs[renderTextWidth - 1] = Glyph('.');

    renderGlyphs[renderTextWidth] = GLYPH_TERMINATOR;

    r.drawGlyphsToBuffer(canvasBox.x, canvasBox.y, renderGlyphs);
}