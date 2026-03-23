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
    Label(GlyphString glyphs);

    Label(std::function<GlyphString()> glyphFunc);

    // Render the label within the given canvas
    void render(Renderer& r, Rect canvasBox) override;

    // Return the preferred size of the label
    Size measure() const override;

private:
    std::variant<GlyphString, std::function<GlyphString()>> _glyphs;

    GlyphString getGlyphs() const;
};

} // namespace widgets
} // namespace ui
