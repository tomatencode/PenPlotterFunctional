#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "Glyph.hpp"

namespace ui {

class GlyphString {
public:
    GlyphString() {}

    GlyphString(Glyph glyph)
        : _data(1, glyph)
    {}

    GlyphString(std::initializer_list<Glyph> glyphs)
        : _data(glyphs)
    {}

    GlyphString(const std::string& text) {
        size_t n = text.size();
        _data.reserve(n);
        for (size_t i = 0; i < n; ++i)
            _data.emplace_back(static_cast<uint8_t>(text[i]));
    }

    GlyphString(const char* text) {
        size_t n = 0;
        while (text[n] != '\0') ++n;
        _data.reserve(n + 1);
        for (size_t i = 0; i < n; ++i)
            _data.emplace_back(static_cast<uint8_t>(text[i]));
    }

    size_t size() const { return _data.size(); }

    Glyph operator[](size_t i) const { return i < size() ? _data[i] : Glyph{}; }

private:
    std::vector<Glyph> _data;
};

} // namespace ui