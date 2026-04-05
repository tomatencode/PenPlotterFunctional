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

    DynamicGlyphString operator+(const GlyphString& other) const
    {
        auto base = _glyphs;
        return DynamicGlyphString([base, other]() -> GlyphString {
            return (base ? base() : GlyphString{}) + other;
        });
    }

    DynamicGlyphString operator+(Glyph glyph) const
    {
        auto base = _glyphs;
        return DynamicGlyphString([base, glyph]() -> GlyphString {
            return (base ? base() : GlyphString{}) + glyph;
        });
    }

    DynamicGlyphString operator+(const DynamicGlyphString& other) const
    {
        auto base = _glyphs;
        auto tail = other._glyphs;
        return DynamicGlyphString([base, tail]() -> GlyphString {
            return (base ? base() : GlyphString{}) + (tail ? tail() : GlyphString{});
        });
    }

    DynamicGlyphString& operator+=(const GlyphString& other)
    {
        auto prev = _glyphs;
        _glyphs = [prev, other]() -> GlyphString {
            return (prev ? prev() : GlyphString{}) + other;
        };
        return *this;
    }

    DynamicGlyphString& operator+=(Glyph glyph)
    {
        auto prev = _glyphs;
        _glyphs = [prev, glyph]() -> GlyphString {
            return (prev ? prev() : GlyphString{}) + glyph;
        };
        return *this;
    }

    DynamicGlyphString& operator+=(const DynamicGlyphString& other)
    {
        auto prev = _glyphs;
        auto tail = other._glyphs;
        _glyphs = [prev, tail]() -> GlyphString {
            return (prev ? prev() : GlyphString{}) + (tail ? tail() : GlyphString{});
        };
        return *this;
    }

private:
    std::function<GlyphString()> _glyphs;
};

} // namespace ui