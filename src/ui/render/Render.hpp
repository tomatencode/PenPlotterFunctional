#ifndef RENDER_HPP
#define RENDER_HPP
#include "config/ui_config.hpp"
#include "hardware/display/LcdDisplay.hpp"
#include "CustomChars.hpp"
#include "Glyph.hpp"

class Renderer
{
public:
    Renderer(LcdDisplay& display);

    void init();

    void clearBuffer();
    void drawGlyphsToBuffer(int x, int y, const Glyph* glyphs);
    void drawTextToBuffer(int x, int y, const char* text);

    void renderToDisplay();

private:
    LcdDisplay& _display;

    Glyph _buffer[LCD_ROWS][LCD_COLS]; 
};

#endif