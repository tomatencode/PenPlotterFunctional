#include "UiManager.hpp"

#include "framework/text/CustomChars.hpp"
#include "screens/HomeScreen.hpp"
#include "screens/PlottingScreen.hpp"

#include <string>

namespace ui {

UiManager::UiManager(JobController& jobController, MotionState& ms, FileManager& fileManager, LcdDisplay& display, RotaryEncoder& encoder, Buzzer& buzzer)
    : _jobController(jobController), _ms(ms), _fileManager(fileManager), _display(display), _encoder(encoder), _buzzer(buzzer), _renderer(display), _router()
{}

void UiManager::init()
{
    _renderer.init();

    // Start on the first screen
    static screens::HomeScreen homeScreen(_jobController, _ms, _fileManager);
    _router.pushScreen(&homeScreen);

    // Register as observer to be notified of job events
    _jobController.registerObserver(this);

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
    state.buttonPressed = _encoder.buttonPressed();
    state.buttonReleased = _encoder.buttonReleased();
    state.buttonDown = _encoder.buttonDown();

    return state;
}

void UiManager::onJobEvent(const JobEvent& event)
{
    if (event == JobEvent::STARTED) {
        String displayFilename = _jobController.getCurrentFile();
        if (displayFilename.startsWith("/")) {
            displayFilename = displayFilename.substring(1);
        }
        
        static screens::PlottingScreen plottingScreen(displayFilename, _jobController, _ms, true);
        _router.pushScreen(&plottingScreen);
        Serial.println("Navigated to PlottingScreen from observer");
    }
}

} // namespace ui