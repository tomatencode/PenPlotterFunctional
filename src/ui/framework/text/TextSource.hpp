#pragma once

#include "Glyph.hpp"

namespace ui {

class TextSource
{
public:
    virtual const Glyph* getGlyphs() const = 0;
    virtual ~TextSource() = default;
};

} // namespace ui