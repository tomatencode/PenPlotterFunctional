#ifndef LcdDisplay_HPP
#define LcdDisplay_HPP

#include <LCD-I2C.h>

class LcdDisplay
{
public:
    LcdDisplay(LCD_I2C& lcd);

    void clear();
    void setCursor(int x, int y);
    void print(const char* text);
    int width();
    int height();
private:
    LCD_I2C& _lcd;
};

#endif