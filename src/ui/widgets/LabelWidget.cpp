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

    int maxWidth = box().w;

    // Temporary glyph buffer
    Glyph glyphs[LCD_COLS + 1]; // enough for whole LCD width + terminator

    // Fill glyph buffer from TextSource
    const Glyph* srcGlyphs = text.getGlyphs(); // TextSource provides glyph array
    int i = 0;
    for (; i < maxWidth && srcGlyphs[i].code != GLYPH_TERMINATOR.code; i++)
    {
        glyphs[i] = srcGlyphs[i];
    }

    // Handle ellipsis if text is longer than maxWidth
    bool addEllipsis = srcGlyphs[i].code != GLYPH_TERMINATOR.code;
    if (addEllipsis && i > 0)
    {
        glyphs[i - 1] = Glyph('.'); // last char becomes ellipsis
    }

    // Fill remaining space with GLYPH_SPACE
    for (; i < maxWidth; i++)
        glyphs[i] = GLYPH_SPACE;

    glyphs[maxWidth] = GLYPH_TERMINATOR;

    // Determine starting X based on alignment
    int textLen = 0;
    while (textLen < maxWidth && glyphs[textLen].code != GLYPH_TERMINATOR.code)
        textLen++;

    int startX = absX;
    switch (align)
    {
        case TextAlign::Left:
            startX = absX;
            break;
        case TextAlign::Center:
            startX = absX + (maxWidth - textLen) / 2;
            break;
        case TextAlign::Right:
            startX = absX + maxWidth - textLen;
            break;
    }

    if (startX < 0) startX = 0;
    if (startX + maxWidth > LCD_COLS) startX = LCD_COLS - maxWidth;

    // Draw glyphs to buffer
    r.drawGlyphsToBuffer(startX, absY, glyphs);
}