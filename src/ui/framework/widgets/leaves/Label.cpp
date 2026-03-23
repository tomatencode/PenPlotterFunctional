#include "Label.hpp"
#include "../../render/Render.hpp"
#include "../../text/textSources/StaticText.hpp"
#include "../../text/textSources/GlyphListText.hpp"

namespace ui {
namespace widgets {

Label::Label(std::unique_ptr<TextSource> textSource)
    : _text(std::move(textSource))
{}

Label::Label(const char* text)
    : _text(std::unique_ptr<TextSource>(new StaticText(text)))
{}

Size Label::measure() const
{
    const Glyph* glyphs = _text->getGlyphs();
    int width = 0;

    // Count glyphs until terminator or max LCD width
    while (glyphs[width].code != GLYPH_TERMINATOR.code && width < LCD_COLS)
        width++;

    return {
        static_cast<uint8_t>(width),
        static_cast<uint8_t>(1) // single line height
    };
}

void Label::render(Renderer& r, Rect canvasBox)
{
    if (canvasBox.w == 0 || canvasBox.h == 0)
        return;

    r.drawGlyphsToBuffer(canvasBox.x, canvasBox.y, _text->getGlyphs());
}

} // namespace widgets
} // namespace ui