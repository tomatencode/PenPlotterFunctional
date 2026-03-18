#include "UI.hpp"

#include "framework/text/CustomChars.hpp"
#include "screens/TestUI.hpp"

#include <string>

UI::UI(LcdDisplay& display, RotaryEncoder& encoder, Buzzer& buzzer)
    : _display(display), _encoder(encoder), _buzzer(buzzer), _renderer(display), _router()
{}

void UI::init()
{
    _renderer.init();

    // Start on the first screen. Replace this with your own screen stack as needed.
    _router.setScreen(createTestScreen());

    Serial.println("UI initialized.");
}

void UI::update()
{
    ui::InputState input = readInputs();
    _router.handleInput(input);

    _router.render(_renderer);
    _renderer.renderToDisplay();
    delay(50); // simple debounce for display updates, adjust as needed
}

ui::InputState UI::readInputs()
{
    ui::InputState state;

    state.encoderDelta = _encoder.getPositionDelta();
    state.buttonState.buttonPressed = _encoder.buttonPressed();
    state.buttonState.buttonReleased = _encoder.buttonReleased();
    state.buttonState.buttonDown = _encoder.buttonDown();

    return state;
}