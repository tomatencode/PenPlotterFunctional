#include "UI.hpp"

#include "text/CustomChars.hpp"
#include "TestUI.hpp"

#include <string>

UI::UI(LcdDisplay& display, RotaryEncoder& encoder, Buzzer& buzzer)
    : _display(display), _encoder(encoder), _buzzer(buzzer), _renderer(display), _router()
{}

void UI::init()
{
    _renderer.init();
    setupTestUI(_router);
}

void UI::update()
{
    InputState input = readInputs();
    _router.handleInput(input);

    _router.render(_renderer);
    _renderer.renderToDisplay();
    delay(50); // simple debounce for display updates, adjust as needed
}

InputState UI::readInputs()
{
    InputState state;

    state.encoderDelta = _encoder.getPositionDelta();
    state.buttonState.buttonPressed = _encoder.buttonPressed();
    state.buttonState.buttonReleased = _encoder.buttonReleased();
    state.buttonState.buttonDown = _encoder.buttonDown();

    return state;
}