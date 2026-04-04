#include "UiOrchestrator.hpp"

#include "framework/text/CustomChars.hpp"
#include "screens/HomeScreen.hpp"
#include "screens/plotting/PlottingScreen.hpp"

#include <string>

namespace ui {

void UiOrchestrator::init()
{
    _renderer.init();

    // Start on the first screen
    auto homeScreen = std::make_unique<screens::HomeScreen>(
        _jobController,
        _motionState,
        _fileManager,
        _wifiStatusProvider,
        _settingsRepository,
        _runtimeSettings
    );

    _router.pushScreen(std::move(homeScreen));

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

void UiOrchestrator::onJobEvent(const JobEventType& event)
{
    if (event.type == JobEvent::STARTED) {
        auto plottingScreen = std::make_unique<screens::PlottingScreen>(
            _jobController,
            _motionState,
            _wifiStatusProvider
        );

        _router.pushScreen(std::move(plottingScreen));
        Serial.println("Navigated to PlottingScreen from observer");
    }
}

} // namespace ui