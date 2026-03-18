#pragma once

#include "../TextSource.hpp"

namespace ui {

class GlyphListText : public TextSource
{
public:
    GlyphListText(const Glyph* glyphs) : _glyphs(glyphs) {}

    const Glyph* getGlyphs() const override
    {
        return _glyphs;
    }

private:
    const Glyph* _glyphs;
};

} // namespace ui