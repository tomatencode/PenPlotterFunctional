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

    GlyphString operator+(const GlyphString& other) const
    {
        GlyphString result;
        result._data.reserve(_data.size() + other._data.size());
        result._data.insert(result._data.end(), _data.begin(), _data.end());
        result._data.insert(result._data.end(), other._data.begin(), other._data.end());
        return result;
    }

    GlyphString operator+(Glyph glyph) const
    {
        GlyphString result;
        result._data.reserve(_data.size() + 1);
        result._data.insert(result._data.end(), _data.begin(), _data.end());
        result._data.push_back(glyph);
        return result;
    }

    GlyphString& operator+=(const GlyphString& other)
    {
        _data.insert(_data.end(), other._data.begin(), other._data.end());
        return *this;
    }

    GlyphString& operator+=(Glyph glyph)
    {
        _data.push_back(glyph);
        return *this;
    }

    // Returns glyphs in [start, end) — clamps to valid range
    GlyphString substr(size_t start, size_t end) const
    {
        if (start >= _data.size()) return {};
        if (end > _data.size()) end = _data.size();
        GlyphString result;
        result._data.assign(_data.begin() + start, _data.begin() + end);
        return result;
    }

    GlyphString substr(size_t start) const
    {
        size_t end = _data.size();
        if (start >= _data.size()) return {};
        if (end > _data.size()) end = _data.size();
        GlyphString result;
        result._data.assign(_data.begin() + start, _data.begin() + end);
        return result;
    }

private:
    std::vector<Glyph> _data;
};

} // namespace ui