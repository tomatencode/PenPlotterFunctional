#pragma once
#include <cstdint>

namespace ui {

struct Glyph
{
    uint8_t code;

    constexpr Glyph(uint8_t c) : code(c) {}
    constexpr Glyph() : code(' ') {}

    bool operator==(const Glyph& other) const { return code == other.code; }
    bool operator!=(const Glyph& other) const { return code != other.code; }
};

} // namespace ui