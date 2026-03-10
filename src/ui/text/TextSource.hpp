#ifndef TEXT_SOURCE_HPP
#define TEXT_SOURCE_HPP

#include "Glyph.hpp"

class TextSource
{
public:
    virtual const Glyph* getGlyphs() const = 0;
    virtual ~TextSource() = default;
};

#endif