#pragma once


#include <functional>

#include "../TextSource.hpp"
#include "config/ui_config.hpp"

#include <cstring>

namespace ui {

class FunctionText : public TextSource
{
public:
    using TextFunc = std::function<const char*()>;

    FunctionText(TextFunc func) : _func(std::move(func)) {}

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

} // namespace ui