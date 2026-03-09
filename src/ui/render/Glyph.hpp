#ifndef GLYPH_HPP
#define GLYPH_HPP
#include <cstdint>

struct Glyph
{
    uint8_t code;

    constexpr Glyph(uint8_t c) : code(c) {}
    constexpr Glyph() : code(' ') {}
};

constexpr Glyph GLYPH_SPACE(' ');
constexpr Glyph GLYPH_TERMINATOR(255);

#endif