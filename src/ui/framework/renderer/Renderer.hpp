#pragma once
#include "config/ui_config.hpp"
#include "hardware/display/LcdDisplay.hpp"
#include "../text/CustomChars.hpp"
#include "../text/Glyph.hpp"
#include "../text/GlyphString.hpp"

namespace ui {

class Renderer
{
public:
    Renderer(LcdDisplay& display);

    void init();

    void clearBuffer();

    void drawTextToBuffer(int x, int y, const char* text);
    void drawGlyphsToBuffer(int x, int y, const GlyphString& s);
    void drawGlyphToBuffer(int x, int y, Glyph g);

    void renderToDisplay();

private:
    LcdDisplay& _display;

    Glyph _buffer[LCD_ROWS][LCD_COLS]; 
    Glyph _currentScreen[LCD_ROWS][LCD_COLS];
};

} // namespace ui