#pragma once

#include <memory>
#include <functional>
#include <variant>

#include "../core/Widget.hpp"
#include "../../text/Glyph.hpp"
#include "../../text/GlyphString.hpp"

namespace ui {
namespace widgets {

class Label : public Widget
{
public:

    // Static label
    Label(GlyphString glyphs);

    // Dynamic label
    Label(std::function<GlyphString()> glyphFunc);

    void render(Renderer& r, Rect canvasBox) override;

    Size measure() const override;

private:
    std::variant<GlyphString, std::function<GlyphString()>> _glyphs;

    GlyphString getGlyphs() const;
};

} // namespace widgets
} // namespace ui
