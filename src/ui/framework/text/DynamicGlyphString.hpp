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
        if (!_glyphs) return GlyphString{};
        return _glyphs();
    }

    size_t size() const {
        if (!_glyphs) return 0;
        return _glyphs().size();
    }

    Glyph operator[](size_t i) const {
        if (!_glyphs) return Glyph{};

        if (i < size())
            return _glyphs()[i];
        else
            return Glyph{};
    }

private:
    std::function<GlyphString()> _glyphs;
};

} // namespace ui