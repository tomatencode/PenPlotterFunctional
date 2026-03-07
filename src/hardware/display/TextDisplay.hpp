#ifndef Displyay_HPP
#define Displyay_HPP

class TextDisplay
{
public:
    virtual void clear() = 0;
    virtual void drawText(int x, int y, const char* text) = 0;
    virtual int width() = 0;
    virtual int height() = 0;
};

#endif