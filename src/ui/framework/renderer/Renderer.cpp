#include "Renderer.hpp"

namespace ui {

Renderer::Renderer(LcdDisplay& display) : _display(display)
{
    for (int y = 0; y < LCD_ROWS; y++)
    {
        for (int x = 0; x < LCD_COLS; x++)
        {
            _buffer[y][x] = Glyph(' ');
            _currentScreen[y][x] = Glyph(' ');
        }
    }
}

void Renderer::init()
{
    // Load all custom chars
    for (int i = 0; i < 8; i++)
    {
        _display.createChar(i, customChars::CUSTOM_CHAR_MAP[i]);
    }
}

void Renderer::clearBuffer()
{
    for (int y = 0; y < LCD_ROWS; y++)
    {
        for (int x = 0; x < LCD_COLS; x++)
        {
            _buffer[y][x] = Glyph(' ');  // clear to space
        }
    }
}

void Renderer::drawTextToBuffer(int x, int y, const char* text)
{
    if (y < 0 || y >= LCD_ROWS) return;

    while (*text && x < LCD_COLS)
    {
        if (x >= 0)
            _buffer[y][x].code = *text;
        x++;
        text++;
    }
}

void Renderer::drawGlyphsToBuffer(int x, int y, const GlyphString& s)
{
    if (y < 0 || y >= LCD_ROWS) return;
    size_t i = 0;
    while (i < s.size() && x < LCD_COLS) {
        if (x >= 0)
            _buffer[y][x] = s[i];
        x++;
        i++;
    }
}

void Renderer::drawGlyphToBuffer(int x, int y, Glyph g)
{
    if (y < 0 || y >= LCD_ROWS || x < 0 || x >= LCD_COLS) return;

    _buffer[y][x] = g;
}

void Renderer::renderToDisplay()
{
    int cursorX = 0;
    int cursorY = 0;
    _display.setCursor(0, 0);

    for (int y = 0; y < LCD_ROWS; y++)
    {
        for (int x = 0; x < LCD_COLS; x++)
        {
            if (_buffer[y][x] != _currentScreen[y][x])
            {
                if (cursorX != x || cursorY != y) {
                    _display.setCursor(x, y);
                    cursorX = x;
                    cursorY = y;
                }

                _display.write(_buffer[y][x].code);
                cursorX = x + 1;

                _currentScreen[y][x] = _buffer[y][x];
            }
        }
    }
}

} // namespace ui