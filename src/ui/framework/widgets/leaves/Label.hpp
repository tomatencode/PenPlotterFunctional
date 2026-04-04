#pragma once

#include <memory>

#include "../core/Widget.hpp"
#include "../../text/Glyph.hpp"
#include "../../text/GlyphStringProvider.hpp"

namespace ui {
namespace widgets {

class Label : public Widget
{
public:

    Label(GlyphStringProvider glyphs)
        : _glyphs(std::move(glyphs))
    {}

    void render(Renderer& r, Box canvasBox) override {
        if (canvasBox.w == 0 || canvasBox.h == 0)
            return;

        auto glyphs = _glyphs.getGlyphs();
        r.drawGlyphsToBuffer(canvasBox.x, canvasBox.y, glyphs);
    };

    Size measure() const override {
        return {
            static_cast<uint16_t>(_glyphs.size()),
            1
        };
    };

private:
    GlyphStringProvider _glyphs;
};

} // namespace widgets
} // namespace ui
