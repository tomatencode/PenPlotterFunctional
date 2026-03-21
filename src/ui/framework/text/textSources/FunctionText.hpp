#pragma once


#include <functional>

#include "../TextSource.hpp"
#include "config/ui_config.hpp"

#include <cstring>

namespace ui {

class FunctionText : public TextSource
{
public:
    using TextFunc = std::function<String()>;

    FunctionText(TextFunc func) : _func(std::move(func)) {}

    const Glyph* getGlyphs() const override
    {
        String text = _func();
        const char* str = text.c_str();
        size_t len = strlen(str);

        // Copy chars into internal glyph buffer
        for (size_t i = 0; i < len && i < MAX_LENGTH; i++)
            _glyphs[i] = Glyph(str[i]);

        // Add terminator
        _glyphs[len] = GLYPH_TERMINATOR;
        return _glyphs;
    }

private:
    static constexpr size_t MAX_LENGTH = LCD_COLS; // adjust if needed
    TextFunc _func;
    mutable Glyph _glyphs[MAX_LENGTH];
};

} // namespace ui