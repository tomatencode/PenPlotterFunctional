#ifndef STATIC_TEXT_HPP
#define STATIC_TEXT_HPP

#include "../TextSource.hpp"
#include "config/ui_config.hpp"

#include <cstring>

class StaticText : public TextSource
{
public:
    StaticText(const char* str)
    {
        size_t len = strlen(str);
        size_t i = 0;

        // Copy chars into internal glyph buffer
        for (; i < len && i < MAX_LENGTH - 1; i++)
            _glyphs[i] = Glyph(static_cast<uint8_t>(str[i]));

        // Add terminator
        _glyphs[i] = GLYPH_TERMINATOR;
    }

    const Glyph* getGlyphs() const override
    {
        return _glyphs;
    }

private:
    static constexpr size_t MAX_LENGTH = LCD_COLS;
    Glyph _glyphs[MAX_LENGTH];
};

#endif