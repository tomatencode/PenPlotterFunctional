#pragma once
#include <cstdint>
#include <vector>
#include <functional>
#include "GlyphString.hpp"

namespace ui {

class DynamicGlyphString {
public:
    DynamicGlyphString() {}

    DynamicGlyphString(std::function<GlyphString()> glyphs)
        : _glyphs(std::move(glyphs))
    {}

    GlyphString getGlyphs() const {
        return _glyphs();
    }

    size_t size() const { return _glyphs().size(); }

    Glyph operator[](size_t i) const { return i < size() ? _glyphs()[i] : GLYPH_NONE; }

private:
    std::function<GlyphString()> _glyphs;
};

} // namespace ui