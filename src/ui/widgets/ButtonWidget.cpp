#include "ButtonWidget.hpp"

ButtonWidget::ButtonWidget(Rect box,
                           Widget* child,
                           ButtonStyle style,
                           std::function<void()> onPress,
                           std::function<void()> onRelease)
    : SelectableWidget(box),
      _child(child),
      _style(style),
      _onPress(onPress),
      _onRelease(onRelease),
      _isPressed(false)
{
}

void ButtonWidget::render(Renderer& r, Rect canvasBox)
{
    Rect widgetBox = box();
    int absX = canvasBox.x + widgetBox.x;
    int absY = canvasBox.y + widgetBox.y;

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

    // Draw left decoration if needed
    int x = absX;
    if (left.code != GLYPH_NONE.code && widgetBox.w > 0)
    {
        r.drawGlyphToBuffer(x, absY, left);
        x++;
    }

    // Draw child
    if (_child)
    {
        Size childSize = _child->measure();
        Rect childCanvas = { 
            static_cast<uint8_t>(x),
            static_cast<uint8_t>(absY),
            static_cast<uint8_t>(childSize.w),
            1 // single line height
        };
        _child->render(r, childCanvas);
        x += childSize.w;
    }

    // Draw right decoration if needed
    if (right.code != GLYPH_NONE.code && widgetBox.w > 1)
        r.drawGlyphToBuffer(x, absY, right);
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
    // Optional: visual or sound feedback
}