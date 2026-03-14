#ifndef GLYPH_LIST_TEXT_HPP
#define GLYPH_LIST_TEXT_HPP

#include "../TextSource.hpp"

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

#endif