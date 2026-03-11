#pragma once
#include <cstdint>

struct Glyph
{
    uint8_t code;

    constexpr Glyph(uint8_t c) : code(c) {}
    constexpr Glyph() : code(' ') {}
};

// Special terminator for glyph arrays
constexpr Glyph GLYPH_TERMINATOR(255);

// Special “no glyph” sentinel for optional decorations
constexpr Glyph GLYPH_NONE(254);

// Space glyph for clearingbuffers
constexpr Glyph GLYPH_SPACE(' ');