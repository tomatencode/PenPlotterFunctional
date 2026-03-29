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

    int x = canvasBox.x;
    int y = canvasBox.y;

    // Choose decorations based on state
    Glyph left  = _style.leftNormal;
    Glyph right = _style.rightNormal;

    if (_isPressed)
    {
        left  = _style.leftPressed;
        right = _style.rightPressed;
    }
    else if (isFocused())
    {
        left  = _style.leftFocused;
        right = _style.rightFocused;
    }

    // Draw left decoration
    if (left.code != GLYPH_NONE.code && canvasBox.w > 0)
    {
        r.drawGlyphToBuffer(x, y, left);
        x++;
    }

    Size childSize = _label->measure();

    // Create a canvas for the child within the button
    Rect childCanvas = {
        x,
        y,
        std::min<int>(childSize.w, canvasBox.w - (x - canvasBox.x)),
        1 // assume single-line height
    };

    _label->render(r, childCanvas);

    x += childSize.w;

    // Draw right decoration
    if (right.code != GLYPH_NONE.code && canvasBox.w > 1)
    {
        r.drawGlyphToBuffer(x, y, right);
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
