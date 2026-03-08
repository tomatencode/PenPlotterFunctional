#include "LcdDisplay.hpp"
#include "config/ui_config.hpp"

LcdDisplay::LcdDisplay(LCD_I2C& lcd) : _lcd(lcd) {}

void LcdDisplay::clear()
{
    _lcd.clear();
}

void LcdDisplay::setCursor(int x, int y)
{
    _lcd.setCursor(x, y);
}

void LcdDisplay::print(const char* text)
{
    _lcd.print(text);
}

void LcdDisplay::write(uint8_t character)
{
    _lcd.write(character);
}

void LcdDisplay::createChar(uint8_t memory_location, uint8_t* charmap)
{
    _lcd.createChar(memory_location, charmap);
}