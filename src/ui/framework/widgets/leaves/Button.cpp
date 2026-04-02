#include "Button.hpp"

namespace ui {
namespace widgets {

Size Button::measure() const
{
    Size childSize = Container::measure();
    uint8_t width = childSize.w;
    if (isFocused())
    {
        if (_props.style.leftFocused.code != GLYPH_NONE.code) width++;
        if (_props.style.rightFocused.code != GLYPH_NONE.code) width++;
    }
    else if (_isPressed)
    {
        if (_props.style.leftPressed.code != GLYPH_NONE.code) width++;
        if (_props.style.rightPressed.code != GLYPH_NONE.code) width++;
    }
    else
    {
        if (_props.style.leftNormal.code != GLYPH_NONE.code) width++;
        if (_props.style.rightNormal.code != GLYPH_NONE.code) width++;
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
    Glyph leftDecorator  = _props.style.leftNormal;
    Glyph rightDecorator = _props.style.rightNormal;

    if (_isPressed)
    {
        leftDecorator  = _props.style.leftPressed;
        rightDecorator = _props.style.rightPressed;
    }
    else if (isFocused())
    {
        leftDecorator  = _props.style.leftFocused;
        rightDecorator = _props.style.rightFocused;
    }

    // Draw left decoration
    if (leftDecorator.code != GLYPH_NONE.code && canvasBox.w > 0)
    {
        r.drawGlyphToBuffer(x, y, leftDecorator);
        x++;
    }

    Size ChildSize = Container::measure();

    if (ChildSize.h > 1)
        Serial.println("Warning: Button label height exceeds 1, clipping will occur");
    
    uint16_t decorationWidth = 0;
    if (leftDecorator.code != GLYPH_NONE.code) decorationWidth++;
    if (rightDecorator.code != GLYPH_NONE.code) decorationWidth++;

    int remainingWidth = static_cast<int>(canvasBox.w) - static_cast<int>(decorationWidth);
    uint16_t finalWidth = std::min<uint16_t>(ChildSize.w, remainingWidth);

    // Create a canvas for the child within the button
    Rect childCanvas = {
        x,
        y,
        finalWidth,
        1
    };

    Container::render(r, childCanvas);

    x += ChildSize.w;

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

    if (input.buttonPressed && _props.onPress)
        _props.onPress();

    if (input.buttonReleased && _props.onRelease)
        _props.onRelease();

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
