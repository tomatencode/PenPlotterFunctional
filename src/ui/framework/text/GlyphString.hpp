#pragma once
#include <cstdint>
#include <vector>
#include "Glyph.hpp"

namespace ui {

class GlyphString {
public:
    GlyphString() {}

    // New: initializer list of Glyph
    GlyphString(std::initializer_list<Glyph> glyphs)
        : _data(glyphs)
    {}

    GlyphString(const String& text) {
        size_t n = text.length();
        _data.reserve(n + 1);
        for (size_t i = 0; i < n; ++i)
            _data.emplace_back(static_cast<uint8_t>(text[i]));
    }

    size_t size() const { return _data.size(); }

    Glyph operator[](size_t i) const { return i < size() ? _data[i] : GLYPH_NONE; }

private:
    std::vector<Glyph> _data;
};

} // namespace ui