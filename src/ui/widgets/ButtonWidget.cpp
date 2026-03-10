#include "ButtonWidget.hpp"

ButtonWidget::ButtonWidget(Rect box,
                           Widget* child,
                           ButtonStyle style,
                           std::function<void()> onPress,
                           std::function<void()> onRelease,
                           Alignment align
                        )
    : SelectableWidget(box, align),
      _child(child),
      _style(style),
      _onPress(onPress),
      _onRelease(onRelease),
      _isPressed(false)
{}

Size ButtonWidget::measure() const
{
    Size childSize = _child ? _child->measure() : Size{0, 0};
    uint8_t width = childSize.w;
    if (_style.leftNormal.code != GLYPH_NONE.code) width++;
    if (_style.rightNormal.code != GLYPH_NONE.code) width++;
    return { width, static_cast<uint8_t>(1) };
}

void ButtonWidget::render(Renderer& r, Rect canvasBox)
{
    // Compute final drawing rectangle including alignment + clipping
    Rect drawRect = computeContentRect(canvasBox);

    if (drawRect.w == 0 || drawRect.h == 0)
        return; // nothing visible

    int x = drawRect.x;
    int y = drawRect.y;

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
    if (left.code != GLYPH_NONE.code && drawRect.w > 0)
    {
        r.drawGlyphToBuffer(x, y, left);
        x++;
    }

    // Draw child if present
    if (_child)
    {
        Size childSize = _child->measure();

        // Create a small canvas for the child within the button
        Rect childCanvas = {
            static_cast<uint8_t>(x),
            static_cast<uint8_t>(y),
            static_cast<uint8_t>(std::min<int>(childSize.w, drawRect.w - (x - drawRect.x))),
            1 // assume single-line height
        };

        _child->render(r, childCanvas);

        x += childSize.w;
    }

    // Draw right decoration
    if (right.code != GLYPH_NONE.code && drawRect.w > 1)
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
    // Optional: visual or sound feedback
}