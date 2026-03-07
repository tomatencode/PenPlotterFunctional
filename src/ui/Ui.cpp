#include "UI.hpp"
#include <string>

int bnt_counter = 0;

UI::UI(LcdDisplay& display, RotaryEncoder& encoder) : _display(display), _encoder(encoder) {}

void UI::update()
{
    int pos = _encoder.getPosition(); // read encoder position to update internal state (if needed)
    bool buttonPressed = _encoder.buttonPressed(); // check if button was pressed to trigger actions (if needed)
    _display.clear();
    _display.setCursor(0, 0);
    _display.print(("Position: " + std::to_string(pos)).c_str());

    if (buttonPressed) {
        bnt_counter++;
    }

    _display.setCursor(0, 1);
    _display.print(("Button Pressed: " + std::to_string(bnt_counter)).c_str());

    delay(100); // Small delay to prevent excessive updates, adjust as needed
}