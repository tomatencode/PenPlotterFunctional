#include "Render.hpp"

Renderer::Renderer(LcdDisplay& display) : _display(display) {}

void Renderer::init()
{
    clearBuffer();

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
            _buffer[y][x].id = ' ';  // clear to space
        }
    }
}

void Renderer::drawGlyphsToBuffer(int x, int y, const Glyph* glyphs)
{
    if (y >= LCD_ROWS) return;

    while (glyphs->id != TERMINATOR.id && x < LCD_COLS)
    {
        _buffer[y][x++] = *glyphs;
        glyphs++;
    }
}

void Renderer::renderToDisplay()
{
    for (int y = 0; y < LCD_ROWS; y++)
    {
        _display.setCursor(0, y);

        for (int x = 0; x < LCD_COLS; x++)
        {
            _display.write(_buffer[y][x].id);
        }
    }
}