#pragma once

#include "Glyph.hpp"

class TextSource
{
public:
    virtual const Glyph* getGlyphs() const = 0;
    virtual ~TextSource() = default;
};