#include "UiManager.hpp"

#include "framework/text/CustomChars.hpp"
#include "screens/HomeScreen.hpp"
#include "screens/PlottingScreen.hpp"

#include <string>

namespace ui {

UiManager::UiManager(JobManager& jobManager, MotionStateManager& ms, LcdDisplay& display, RotaryEncoder& encoder, Buzzer& buzzer)
    : _jobManager(jobManager), _ms(ms), _display(display), _encoder(encoder), _buzzer(buzzer), _renderer(display), _router()
{}

void UiManager::init()
{
    _renderer.init();

    // Start on the first screen
    static screens::HomeScreen homeScreen(_jobManager, _ms);
    _router.pushScreen(&homeScreen);

    // Register as observer to be notified of job events
    _jobManager.registerObserver(this);

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

void UiManager::onJobEvent(const JobStatusUpdate& update)
{
    Serial.println("UI received job event: " + String(static_cast<int>(update.eventType)));
    
    if (update.eventType == JobEventType::STARTED) {
        // Navigate to PlottingScreen when a job starts (from any source)
        String displayFilename = _jobManager.getCurrentFile();
        if (displayFilename.startsWith("/")) {
            displayFilename = displayFilename.substring(1);
        }
        
        // Create a new PlottingScreen with the current job info
        // Pass alreadyStarted=true because the job was started by JobManager
        static screens::PlottingScreen plottingScreen(displayFilename, _jobManager, _ms, true);
        _router.pushScreen(&plottingScreen);
        Serial.println("Navigated to PlottingScreen from observer");
    }
}

} // namespace ui