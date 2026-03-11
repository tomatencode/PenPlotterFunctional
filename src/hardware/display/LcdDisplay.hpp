#pragma once
#include <LCD-I2C.h>

class LcdDisplay
{
public:
    LcdDisplay(LCD_I2C& lcd);

    void clear();
    void setCursor(int x, int y);
    void print(const char* text);
    void write(uint8_t character);
    void createChar(uint8_t memory_location, uint8_t* charmap);
private:
    LCD_I2C& _lcd;
};