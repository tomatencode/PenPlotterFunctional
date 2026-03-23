#include "PressHoldButton.hpp"

namespace ui {
namespace components {

void PressHoldButton::handleInput(InputState& input)
{
    if (!isFocused()) return;

    if (input.buttonPressed)
    {
        _pressStartTime = millis();
    }
    else if (input.buttonReleased)
    {
        uint32_t pressDuration = millis() - _pressStartTime;
        if (pressDuration >= _holdTimeMs && _onHoldRelease)
        {
            _onHoldRelease();
        }
    }

    input.buttonPressed = false; // consume press event
    input.buttonReleased = false; // consume release event

    // Call base class to handle visual state changes
    Button::handleInput(input);
}

void PressHoldButton::render(Renderer& r, widgets::Rect canvasBox)
{
    // Call base class to render button and child
    Button::render(r, canvasBox);

    // If button is pressed, draw hold countdown over the button
    if (!isFocused() || !isPressed()) return;

    uint8_t x = canvasBox.x + 1; // Start just after left decoration
    uint8_t y = canvasBox.y;


    uint32_t elapsed = millis() - _pressStartTime;

    // Calculate button width excluding decorations
    uint8_t buttonWidth = 0;
    if (Button::canExpandHorizontally())
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