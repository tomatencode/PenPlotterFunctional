#include "Button.hpp"

namespace ui {
namespace widgets {

Size Button::measure() const
{
    Size childSize = _label->measure();
    uint8_t width = childSize.w;
    if (isFocused())
    {
        if (_style.leftFocused.code != GLYPH_NONE.code) width++;
        if (_style.rightFocused.code != GLYPH_NONE.code) width++;
    }
    else if (_isPressed)
    {
        if (_style.leftPressed.code != GLYPH_NONE.code) width++;
        if (_style.rightPressed.code != GLYPH_NONE.code) width++;
    }
    else
    {
        if (_style.leftNormal.code != GLYPH_NONE.code) width++;
        if (_style.rightNormal.code != GLYPH_NONE.code) width++;
    }
    return { width, 1 };
}

void Button::render(Renderer& r, Rect canvasBox)
{
    // Button renders at the given canvasBox position, full width
    if (canvasBox.w == 0 || canvasBox.h == 0)
        return; // nothing visible

    uint16_t x = canvasBox.x;
    uint16_t y = canvasBox.y;

    // Choose decorations based on state
    Glyph leftDecorator  = _style.leftNormal;
    Glyph rightDecorator = _style.rightNormal;

    if (_isPressed)
    {
        leftDecorator  = _style.leftPressed;
        rightDecorator = _style.rightPressed;
    }
    else if (isFocused())
    {
        leftDecorator  = _style.leftFocused;
        rightDecorator = _style.rightFocused;
    }

    // Draw left decoration
    if (leftDecorator.code != GLYPH_NONE.code && canvasBox.w > 0)
    {
        r.drawGlyphToBuffer(x, y, leftDecorator);
        x++;
    }

    Size labelSize = _label->measure();

    if (labelSize.h > 1)
        Serial.println("Warning: Button label height exceeds 1, clipping will occur");
    
    uint16_t decorationWidth = 0;
    if (leftDecorator.code != GLYPH_NONE.code) decorationWidth++;
    if (rightDecorator.code != GLYPH_NONE.code) decorationWidth++;

    int remainingWidth = static_cast<int>(canvasBox.w) - static_cast<int>(decorationWidth);
    uint16_t finalWidth = std::min<uint16_t>(labelSize.w, remainingWidth);

    // Create a canvas for the child within the button
    Rect childCanvas = {
        x,
        y,
        finalWidth,
        1
    };

    _label->render(r, childCanvas);

    x += labelSize.w;

    // Draw right decoration
    if (rightDecorator.code != GLYPH_NONE.code && canvasBox.w > 1)
    {
        r.drawGlyphToBuffer(x, y, rightDecorator);
    }
}

void Button::handleInput(InputState& input)
{
    if (!isFocused()) return;

    _isPressed = input.buttonDown;

    if (input.buttonPressed && _onPress)
        _onPress();

    if (input.buttonReleased && _onRelease)
        _onRelease();

    input.buttonPressed = false; // consume press event
}

void Button::onFocusGained()
{
    // Optional: visual or sound feedback
}

void Button::onFocusLost()
{
    _isPressed = false; // reset pressed state when losing focus
}

} // namespace widgets
} // namespace ui
