#include "Render.hpp"

Renderer::Renderer(LcdDisplay& display) : _display(display)
{
    clearBuffer();
}

void Renderer::init()
{
    // Load all custom chars
    for (int i = 0; i < 8; i++)
    {
        _display.createChar(i, CUSTOM_CHAR_MAP[i]);
    }
}

void Renderer::clearBuffer()
{
    for (int y = 0; y < LCD_ROWS; y++)
    {
        for (int x = 0; x < LCD_COLS; x++)
        {
            _buffer[y][x] = GLYPH_SPACE;  // clear to space
        }
    }
}

void Renderer::drawGlyphsToBuffer(int x, int y, const Glyph* glyphs)
{
    if (y >= LCD_ROWS) return;

    while (glyphs->code != GLYPH_TERMINATOR.code && x < LCD_COLS)
    {
        if (glyphs->code != GLYPH_NONE.code) // skip "no glyph"
            _buffer[y][x] = *glyphs;
        x++;
        glyphs++;
    }
}

void Renderer::drawGlyphToBuffer(int x, int y, Glyph g)
{
    if (y >= LCD_ROWS || x >= LCD_COLS) return;
    if (g.code != GLYPH_NONE.code)
        _buffer[y][x] = g;
}

void Renderer::drawTextToBuffer(int x, int y, const char* text)
{
    if (y >= LCD_ROWS) return;

    while (*text && x < LCD_COLS)
    {
        _buffer[y][x++].code = *text++;
    }
}

void Renderer::renderToDisplay()
{
    for (int y = 0; y < LCD_ROWS; y++)
    {
        _display.setCursor(0, y);

        for (int x = 0; x < LCD_COLS; x++)
        {
            _display.write(_buffer[y][x].code);
        }
    }
}