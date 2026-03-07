#include "LcdDisplay.hpp"
#include "config/ui_config.hpp"

LcdDisplay::LcdDisplay(LCD_I2C& lcd) : _lcd(lcd) {}

void LcdDisplay::clear()
{
    _lcd.clear();
}

void LcdDisplay::drawText(int x, int y, const char* text)
{
    _lcd.setCursor(x, y);
    _lcd.print(text);
}

int LcdDisplay::width()
{
    return LCD_COLS;
}

int LcdDisplay::height()
{
    return LCD_ROWS; 
}
