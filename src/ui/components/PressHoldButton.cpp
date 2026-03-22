#include "PressHoldButton.hpp"

namespace ui {
namespace components {

PressHoldButton::PressHoldButton(std::unique_ptr<Widget> child, PressHoldButtonStyle style ,std::function<void()> onHoldRelease, uint16_t holdTimeMs)
    : ButtonWidget(std::move(child), widgets::ButtonStyle{
        .leftNormal = style.leftNormal,
        .rightNormal = style.rightNormal,
        .leftFocused = style.leftFocused,
        .rightFocused = style.rightFocused,
        .leftPressed = style.leftPressed,
        .rightPressed = style.rightPressed
    }, nullptr, nullptr)
    , _onHoldRelease(std::move(onHoldRelease))
    , _holdTimeMs(holdTimeMs)
    , _style(std::move(style))
{
}

void PressHoldButton::handleInput(InputState& input)
{
    if (!isFocused()) return;

    if (input.buttonState.buttonPressed)
    {
        _pressStartTime = millis();
    }
    else if (input.buttonState.buttonReleased)
    {
        uint32_t pressDuration = millis() - _pressStartTime;
        if (pressDuration >= _holdTimeMs && _onHoldRelease)
        {
            _onHoldRelease();
        }
    }

    input.buttonState.buttonPressed = false; // consume press event
    input.buttonState.buttonReleased = false; // consume release event

    // Call base class to handle visual state changes
    ButtonWidget::handleInput(input);
}

void PressHoldButton::render(Renderer& r, widgets::Rect canvasBox)
{
    // Call base class to render button and child
    ButtonWidget::render(r, canvasBox);

    // If button is pressed, draw hold countdown over the button
    if (!isFocused() || !isPressed()) return;

    uint8_t x = canvasBox.x + 1; // Start just after left decoration
    uint8_t y = canvasBox.y;


    uint32_t elapsed = millis() - _pressStartTime;

    // Calculate button width excluding decorations
    uint8_t buttonWidth = 0;
    if (ButtonWidget::canExpandHorizontally())
        buttonWidth = canvasBox.w - 2; // Exclude decorations
    else
        buttonWidth = measure().w - 2; // Exclude decorations

    // Calculate how much of the button width to fill based on time left
    uint8_t fillWidth = buttonWidth * std::min(elapsed, static_cast<uint32_t>(_holdTimeMs)) / _holdTimeMs;

    // Draw filled portion
    for (uint8_t i = 0; i < fillWidth; i++)
    {
        r.drawGlyphToBuffer(x + i, y,_style.HoldCountdownFilled);
    }
    for (uint8_t i = fillWidth; i < buttonWidth; i++)
    {
        r.drawGlyphToBuffer(x + i, y, _style.HoldCountdownEmpty);
    }
}


} // namespace components
} // namespace ui
