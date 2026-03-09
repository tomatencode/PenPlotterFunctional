#include "UI.hpp"
#include "render/CustomChars.hpp"
#include <string>

int bnt_prs_counter = 0;
int bnt_rel_counter = 0;

UI::UI(LcdDisplay& display, RotaryEncoder& encoder, Buzzer& buzzer)
    : _display(display), _encoder(encoder), _buzzer(buzzer), _renderer(display)
{}

void UI::init()
{
    _renderer.init();
}

void UI::update()
{
    int pos = _encoder.getPosition();
    bool buttonPressed = _encoder.buttonPressed();

    _renderer.clearBuffer();

    // ---- Position ----
    std::string posStr = "Position: " + std::to_string(pos);
    _renderer.drawTextToBuffer(0, 0, posStr.c_str());

    // ---- Button Press ----
    if (buttonPressed)
    {
        _buzzer.beep(1000, 100);
        bnt_prs_counter++;
    }

    std::string btnStr = "Button Pressed: " + std::to_string(bnt_prs_counter);
    _renderer.drawTextToBuffer(0, 1, btnStr.c_str());

    // ---- Button Release ----
    if (_encoder.buttonReleased())
    {
        bnt_rel_counter++;
    }

    std::string btnRelStr = "Button Released: " + std::to_string(bnt_rel_counter);
    _renderer.drawTextToBuffer(0, 2, btnRelStr.c_str());

    // ---- Button Down ----
    std::string btnDownStr = "Button Down: " + std::to_string(_encoder.buttonDown());
    _renderer.drawTextToBuffer(0, 3, btnDownStr.c_str());

    _renderer.renderToDisplay();

    delay(100);
}

InputState UI::readInputs()
{
    InputState state;

    state.encoderDelta = _encoder.getPosition();
    state.buttonPressed = _encoder.buttonPressed();
    state.buttonReleased = _encoder.buttonReleased();
    state.buttonDown = _encoder.buttonDown();

    _encoder.reset();

    return state;
}