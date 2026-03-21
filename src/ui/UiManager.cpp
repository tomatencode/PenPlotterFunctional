#include "UiManager.hpp"

#include "framework/text/CustomChars.hpp"
#include "screens/HomeScreen.hpp"

#include <string>

namespace ui {

UiManager::UiManager(JobManager& jobManager, LcdDisplay& display, RotaryEncoder& encoder, Buzzer& buzzer)
    : _jobManager(jobManager), _display(display), _encoder(encoder), _buzzer(buzzer), _renderer(display), _router()
{}

void UiManager::init()
{
    _renderer.init();

    // Start on the first screen
    static screens::HomeScreen homeScreen;
    _router.pushScreen(&homeScreen);

    Serial.println("UI initialized.");
}

void UiManager::update()
{
    // Simple non-blocking timing to limit update frequency (e.g., 20 FPS)
    unsigned long currentTime = millis();
    if (currentTime - _lastUpdateTime < 50) return;

    InputState input = readInputs();
    _router.handleInput(input);

    // render current screen to buffer
    _router.render(_renderer);

    // push buffer to display
    _renderer.renderToDisplay();
}

ui::InputState UiManager::readInputs()
{
    ui::InputState state;

    state.encoderDelta = _encoder.getPositionDelta();
    state.buttonState.buttonPressed = _encoder.buttonPressed();
    state.buttonState.buttonReleased = _encoder.buttonReleased();
    state.buttonState.buttonDown = _encoder.buttonDown();

    return state;
}

} // namespace ui