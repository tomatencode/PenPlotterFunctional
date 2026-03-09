#include "UI.hpp"

#include "render/CustomChars.hpp"
#include "TestUI.hpp"

#include <string>

int bnt_prs_counter = 0;
int bnt_rel_counter = 0;

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

    delay(100);
}

InputState UI::readInputs()
{
    InputState state;

    state.encoderDelta = _encoder.getPosition();
    state.buttonState.buttonPressed = _encoder.buttonPressed();
    state.buttonState.buttonReleased = _encoder.buttonReleased();
    state.buttonState.buttonDown = _encoder.buttonDown();

    _encoder.reset();

    return state;
}