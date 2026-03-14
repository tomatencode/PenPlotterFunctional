#ifndef FUNCTION_TEXT_HPP
#define FUNCTION_TEXT_HPP

#include "../TextSource.hpp"
#include "config/ui_config.hpp"

#include <cstring>

class FunctionText : public TextSource
{
public:
    using TextFunc = const char* (*)();

    FunctionText(TextFunc func) : _func(func) {}

    const Glyph* getGlyphs() const override
    {
        const char* str = _func();
        size_t len = strlen(str);
        size_t i = 0;

        // Copy chars into internal glyph buffer
        for (; i < len && i < MAX_LENGTH - 1; i++)
            _glyphs[i] = Glyph(static_cast<uint8_t>(str[i]));

        // Add terminator
        _glyphs[i] = GLYPH_TERMINATOR;
        return _glyphs;
    }

private:
    static constexpr size_t MAX_LENGTH = LCD_COLS; // adjust if needed
    TextFunc _func;
    mutable Glyph _glyphs[MAX_LENGTH];
};

#endif