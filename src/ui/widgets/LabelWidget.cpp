#include "LabelWidget.hpp"
#include "../render/Render.hpp"

LabelWidget::LabelWidget(Rect box, TextSource& textSource, TextAlign align)
    : Widget(box), text(textSource), align(align)
{}

Size LabelWidget::measure() const
{
    const Glyph* glyphs = text.getGlyphs();
    int width = 0;

    // Count glyphs until terminator or max LCD width
    while (glyphs[width].code != GLYPH_TERMINATOR.code && width < box().w)
        width++;

    return {
        static_cast<uint8_t>(width),
        static_cast<uint8_t>(1) // single line height
    };
}

void LabelWidget::render(Renderer& r, Rect canvasBox)
{
    int absX = canvasBox.x + box().x;
    int absY = canvasBox.y + box().y;

    int maxWidth = std::min(box().w, static_cast<uint8_t>(std::abs(canvasBox.w - box().x)));

    const Glyph* srcGlyphs = text.getGlyphs();

    int textWidth = 0;
    while (srcGlyphs[textWidth].code != GLYPH_TERMINATOR.code && textWidth < maxWidth)
        textWidth++;

    if (textWidth == 0)
        return; // Nothing to render

    int renderTextWidth = std::min(textWidth, maxWidth);

    Glyph renderGlyphs[LCD_COLS + 1]; // +1 for terminator
    for (int i = 0; i < renderTextWidth; i++) {
        renderGlyphs[i] = srcGlyphs[i]; // copy glyphs to render buffer
    }

    if (renderTextWidth == 0)
        return; // No glyphs to render

    // Indicate truncation if text exceeds max width
    if (textWidth > maxWidth)
        renderGlyphs[renderTextWidth-1] = Glyph('.');

    // add terminator
    renderGlyphs[renderTextWidth] = GLYPH_TERMINATOR;

    // Calculate starting X based on alignment
    int startX = absX;
    if (align == TextAlign::Center)
        startX += (box().w - renderTextWidth) / 2;
    else if (align == TextAlign::Right)
        startX += box().w - renderTextWidth;

    // Draw glyphs to buffer
    r.drawGlyphsToBuffer(startX, absY, renderGlyphs);
}