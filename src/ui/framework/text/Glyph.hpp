#pragma once
#include <cstdint>

namespace ui {

struct Glyph
{
    uint8_t code;

    constexpr Glyph(uint8_t c) : code(c) {}
    constexpr Glyph() : code(' ') {}
};

// Special “no glyph” sentinel for optional decorations
constexpr Glyph GLYPH_NONE(255);

} // namespace ui