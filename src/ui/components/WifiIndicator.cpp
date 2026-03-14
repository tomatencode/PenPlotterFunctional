#include "WifiIndicator.hpp"
#include "webInterface/WebInterface.hpp"

extern WebInterface webInterface;

WifiIndicator::WifiIndicator() : Widget() {}

void WifiIndicator::render(Renderer& r, Rect canvasBox)
{   
    if (canvasBox.w == 0 || canvasBox.h == 0)
        return; // nothing visible
    
    // check wifi status from shared data and choose symbol

    Glyph wifiSymbol[2];
    wifiSymbol[0] = webInterface.isWiFiConnected() ? Glyph('W') : GLYPH_SPACE;
    wifiSymbol[1] = GLYPH_TERMINATOR; // null-terminate the glyph array

    r.drawGlyphsToBuffer(canvasBox.x, canvasBox.y, wifiSymbol);
}

Size WifiIndicator::measure() const
{
    return Size{1, 1}; // always occupies 1 character width and 1 row height
}