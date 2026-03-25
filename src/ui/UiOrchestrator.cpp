#include "UiOrchestrator.hpp"

#include "framework/text/CustomChars.hpp"
#include "screens/HomeScreen.hpp"
#include "screens/PlottingScreen.hpp"

#include <string>

namespace ui {

void UiOrchestrator::init()
{
    _renderer.init();

    // Start on the first screen
    static screens::HomeScreen homeScreen(_jobController, _motionState, _fileManager);
    _router.pushScreen(&homeScreen);

    // Register as observer to be notified of job events
    _jobController.registerObserver(this);

    Serial.println("UI initialized.");
}

void UiOrchestrator::update()
{
    // Simple non-blocking timing to limit update frequency (e.g., 20 FPS)
    unsigned long currentTime = millis();
    if (currentTime - _lastUpdateTime < 50) return;

    InputState input = _inputMapper.mapInputs();
    _router.handleInput(input);

    // render current screen to buffer
    _router.render(_renderer);

    // push buffer to display
    _renderer.renderToDisplay();
}

void UiOrchestrator::onJobEvent(const JobEvent& event)
{
    if (event == JobEvent::STARTED) {
        String displayFilename = _jobController.getCurrentFile();
        if (displayFilename.startsWith("/")) {
            displayFilename = displayFilename.substring(1);
        }
        
        screens::PlottingScreen plottingScreen(displayFilename, _jobController, _motionState, true);
        _router.pushScreen(&plottingScreen);
        Serial.println("Navigated to PlottingScreen from observer");
    }
}

} // namespace ui