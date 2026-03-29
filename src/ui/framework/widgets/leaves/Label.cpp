#include "Label.hpp"
#include "../../render/Render.hpp"

namespace ui {
namespace widgets {

Label::Label(GlyphString glyphs)
    : _glyphs(std::move(glyphs))
{}

Label::Label(std::function<GlyphString()> glyphFunc)
    : _glyphs(std::move(glyphFunc))
{}

GlyphString Label::getGlyphs() const
{
    if (std::holds_alternative<GlyphString>(_glyphs)) {
        return std::get<GlyphString>(_glyphs);
    } else {
        return std::get<std::function<GlyphString()>>(_glyphs)();
    }
}

Size Label::measure() const
{
    auto glyphs = getGlyphs();
    return {
        static_cast<uint16_t>(glyphs.size()),
        1
    };
}

void Label::render(Renderer& r, Rect canvasBox)
{
    if (canvasBox.w == 0 || canvasBox.h == 0)
        return;

    auto glyphs = getGlyphs();
    r.drawGlyphsToBuffer(canvasBox.x, canvasBox.y, glyphs);
}

} // namespace widgets
} // namespace ui