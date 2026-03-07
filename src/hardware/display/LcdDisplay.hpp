#ifndef LcdDisplay_HPP
#define LcdDisplay_HPP

#include "TextDisplay.hpp"
#include <LCD-I2C.h>

class LcdDisplay : public TextDisplay
{
public:
    LcdDisplay(LCD_I2C& lcd);

    void clear() override;
    void drawText(int x, int y, const char* text) override;
    int width() override;
    int height() override;
private:
    LCD_I2C& _lcd;
};

#endif