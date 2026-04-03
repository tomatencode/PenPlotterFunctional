#include "Button.hpp"

namespace ui {
namespace widgets {

Size Button::measure() const
{
    Size childSize = Container::measure();
    uint8_t width = childSize.w;
    if (_isPressed)
    {
        width += _props.style.leftPressed.size() + _props.style.rightPressed.size();
    }
    else if (isFocused())
    {
        width += _props.style.leftFocused.size() + _props.style.rightFocused.size();
    }
    else
    {
        width += _props.style.leftNormal.size() + _props.style.rightNormal.size();
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
    GlyphString leftDecorator  = _props.style.leftNormal.getGlyphs();
    GlyphString rightDecorator = _props.style.rightNormal.getGlyphs();

    if (_isPressed)
    {
        leftDecorator  = _props.style.leftPressed.getGlyphs();
        rightDecorator = _props.style.rightPressed.getGlyphs();
    }
    else if (isFocused())
    {
        leftDecorator  = _props.style.leftFocused.getGlyphs();
        rightDecorator = _props.style.rightFocused.getGlyphs();
    }

    // Draw left decoration
    r.drawGlyphsToBuffer(x, y, leftDecorator);
    x += leftDecorator.size();

    Size ChildSize = Container::measure();

    if (ChildSize.h > 1)
        Serial.println("Warning: Button label height exceeds 1, clipping will occur");
    
    uint16_t decorationWidth = leftDecorator.size() + rightDecorator.size();

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
    r.drawGlyphsToBuffer(x, y, rightDecorator);
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
