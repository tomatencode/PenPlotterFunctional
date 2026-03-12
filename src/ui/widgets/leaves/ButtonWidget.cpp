#include "ButtonWidget.hpp"
#include "../core/WidgetUtils.hpp"

ButtonWidget::ButtonWidget(Widget* child,
                           ButtonStyle style,
                           void (*onPress)(),
                           void (*onRelease)()
                        )
    : _child(child),
      _style(style),
      _onPress(onPress),
      _onRelease(onRelease),
      _isPressed(false)
{}

Size ButtonWidget::measure() const
{
    Size childSize = _child ? _child->measure() : Size{0, 0};
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
    return { width, static_cast<uint8_t>(1) };
}

void ButtonWidget::render(Renderer& r, Rect canvasBox)
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

    // Draw child if present
    if (_child)
    {
        Size childSize = _child->measure();

        // Create a canvas for the child within the button
        Rect childCanvas = {
            static_cast<uint8_t>(x),
            static_cast<uint8_t>(y),
            static_cast<uint8_t>(std::min<int>(childSize.w, canvasBox.w - (x - canvasBox.x))),
            1 // assume single-line height
        };

        _child->render(r, childCanvas);

        x += childSize.w;
    }

    // Draw right decoration
    if (right.code != GLYPH_NONE.code && canvasBox.w > 1)
    {
        r.drawGlyphToBuffer(x, y, right);
    }
}

void ButtonWidget::handleInput(InputState& input)
{
    if (!isFocused()) return;

    _isPressed = input.buttonState.buttonDown;

    if (input.buttonState.buttonPressed && _onPress)
        _onPress();

    if (input.buttonState.buttonReleased && _onRelease)
        _onRelease();
}

void ButtonWidget::onFocusGained()
{
    // Optional: visual or sound feedback
}

void ButtonWidget::onFocusLost()
{
    _isPressed = false; // reset pressed state when losing focus
}