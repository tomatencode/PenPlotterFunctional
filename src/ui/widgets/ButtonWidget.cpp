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
    Rect widgetBox = box();
    int absX = canvasBox.x + widgetBox.x;
    int absY = canvasBox.y + widgetBox.y;

    Size ownSize = measure();

    int StartX = absX;
    if (align().horizontal == HorizontalAlignment::Center)
        StartX += (widgetBox.w - ownSize.w) / 2;
    else if (align().horizontal == HorizontalAlignment::Right)
        StartX += widgetBox.w - ownSize.w;
    
    int StartY = absY;
    if (align().vertical == VerticalAlignment::Middle)
        StartY += (widgetBox.h - ownSize.h) / 2;
    else if (align().vertical == VerticalAlignment::Bottom)
        StartY += widgetBox.h - ownSize.h;

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
    int x = StartX;
    if (left.code != GLYPH_NONE.code && widgetBox.w > 0)
    {
        r.drawGlyphToBuffer(x, StartY, left);
        x++;
    }

    // Draw child
    if (_child)
    {
        Size childSize = _child->measure();
        Rect childCanvas = { 
            static_cast<uint8_t>(x),
            static_cast<uint8_t>(StartY),
            static_cast<uint8_t>(childSize.w),
            1 // single line height
        };
        _child->render(r, childCanvas);
        x += childSize.w;
    }

    // Draw right decoration if needed
    if (right.code != GLYPH_NONE.code && widgetBox.w > 1)
        r.drawGlyphToBuffer(x, StartY, right);
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