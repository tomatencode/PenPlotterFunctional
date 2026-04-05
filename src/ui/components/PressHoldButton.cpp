#include "PressHoldButton.hpp"

namespace ui {
namespace components {

void PressHoldButton::handleInput(InputState& input)
{
    if (!isFocused()) return;

    int32_t currentTime = millis();

    if (input.buttonPressed)
    {
        _lastPressStartTime = currentTime;
        _lastReleaseStartTime = 0;
        _inRelAnimation = false;
        _callbackPending = false; // Reset any pending callback
    }
    else if (input.buttonReleased)
    {
        uint32_t pressDuration = currentTime - _lastPressStartTime;
        _lastReleaseStartTime = currentTime;
        _inRelAnimation = true;

        if (pressDuration >= _holdTimeMs)
        {
            _callbackPending = true;
        }
    }

    // Check if release animation completed and callback is pending
    if (_lastReleaseStartTime > 0 && currentTime - _lastReleaseStartTime >= _relAnimationTimeMs)
    {
        _lastReleaseStartTime = 0;
        _inRelAnimation = false;

        if (_callbackPending) {
            _callbackPending = false;

            if (_onHoldRelease)
                _onHoldRelease();
        }
    }

    input.buttonPressed = false; // consume press event
    input.buttonReleased = false; // consume release event

    // Call base class to handle visual state changes
    Button::handleInput(input);
}

void PressHoldButton::render(Renderer& r, widgets::Box canvasBox)
{
    // Call base class to render button and child
    Button::render(r, canvasBox);

    if (!isFocused() || !(isPressed() || _inRelAnimation)) return;

    uint32_t currentTime = millis();

    int16_t x = canvasBox.x + 1; // Start just after left decoration
    int16_t y = canvasBox.y;

    // Calculate button width excluding decorations
    uint8_t buttonWidth = 0;
    if (Button::canExpandHorizontally())
        buttonWidth = canvasBox.w - 2; // Exclude decorations
    else
        buttonWidth = measure().w - 2; // Exclude decorations

    uint32_t timeHeldMs;
    if (isPressed()) {
        timeHeldMs = currentTime - _lastPressStartTime;
    }
    else {
        uint32_t timeReleased = _lastReleaseStartTime > 0 ? currentTime - _lastReleaseStartTime : 0;
        timeHeldMs = currentTime - _lastPressStartTime - timeReleased;
    }

    uint8_t fillWidth = buttonWidth * std::min(timeHeldMs, static_cast<uint32_t>(_holdTimeMs)) / _holdTimeMs;

    for (uint8_t i = 0; i < fillWidth; i++)
    {
        if (_inRelAnimation)
            r.drawGlyphsToBuffer(x + i, y, _style.HoldReleaseAnimation);
        else
            r.drawGlyphsToBuffer(x + i, y, _style.HoldCountdownFilled);
    }
    for (uint8_t i = fillWidth; i < buttonWidth; i++)
    {
        r.drawGlyphsToBuffer(x + i, y, _style.HoldCountdownEmpty);
    }
}

} // namespace components
} // namespace ui