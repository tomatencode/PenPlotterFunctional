#include "ButtonWidget.hpp"

ButtonWidget::ButtonWidget(Rect box, const char* label, void (*onPress)(), void (*onRelease)())
    : SelectableWidget(box), label(label), onPress(onPress), onRelease(onRelease) {}

void ButtonWidget::render(Renderer& r, Rect canvasBox)
{
    // Calculate absolute position on the display
    int absX = canvasBox.x + box().x;
    int absY = canvasBox.y + box().y;

    int maxChars = box().w; // assuming 1 char per column
    char Text[maxChars + 1];

    // add visual indication for focus (e.g., brackets)
    if (isFocused())
    {
        if (_isPressed)
        {
            snprintf(Text, maxChars + 1, "<%s>", label);
        }
        else
        {
            snprintf(Text, maxChars + 1, "-%s-", label);
        }
    } else {
        snprintf(Text, maxChars + 1, "[%s]", label);
    }

    r.drawTextToBuffer(absX, absY, Text);
}

void ButtonWidget::handleInput(InputState& input)
{
    if (!isFocused()) return;

    _isPressed = input.buttonState.buttonDown;

    if (input.buttonState.buttonPressed)
    {
        if (onPress) onPress();
    }
    else if (input.buttonState.buttonReleased)
    {
        if (onRelease) onRelease();
    }
}

void ButtonWidget::onFocusGained()
{
    // Optional: could add visual feedback or sound here
}

void ButtonWidget::onFocusLost()
{
    // Optional: could add visual feedback or sound here
}