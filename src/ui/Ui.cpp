#include "UI.hpp"
#include <string>

UI::UI(TextDisplay& display, RotaryEncoder& encoder) : _display(display), _encoder(encoder) {}

void UI::update()
{
    int pos = _encoder.getPosition(); // read encoder position to update internal state (if needed)
    bool buttonPressed = _encoder.buttonPressed(); // check if button was pressed to trigger actions (if needed)
    _display.clear();
    _display.drawText(0, 0, ("Position: " + std::to_string(pos)).c_str());

    if (buttonPressed) {
        _display.drawText(0, 1, "Button Pressed!");
    }

    delay(100); // Small delay to prevent excessive updates, adjust as needed
}