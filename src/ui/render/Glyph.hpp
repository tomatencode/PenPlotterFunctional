#ifndef GLYPH_HPP
#define GLYPH_HPP
#include <cstdint>

struct Glyph
{
    uint8_t id;
};

const Glyph TERMINATOR = {255};

#endif