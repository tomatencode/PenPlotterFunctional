#include "UiOrchestrator.hpp"

#include "framework/text/CustomChars.hpp"
#include "screens/HomeScreen.hpp"
#include "screens/plotting/PlottingScreen.hpp"

#include <string>
#include <esp_log.h>

static const char* TAG = "UiOrchestrator";

namespace ui {

void UiOrchestrator::init()
{
    _renderer.init();

    // Push initial screen
    auto homeScreen = std::make_unique<screens::HomeScreen>(_screensCtx);
    _router.pushScreen(std::move(homeScreen));

    _jobController.registerObserver(this);

    ESP_LOGI(TAG, "UI initialized");
}

UiOrchestrator::~UiOrchestrator() {
    _jobController.unregisterObserver(this);
}

void UiOrchestrator::update()
{
    // non-blocking timing to limit update frequency 20 FPS
    unsigned long currentTime = millis();
    if (currentTime - _lastUpdateTime < 50) return;

    InputState input = _inputMapper.mapInputs();
    _router.handleInput(input);

    _router.render(_renderer); // render current screen to buffer
    _renderer.renderToDisplay(); // push buffer to display
}

void UiOrchestrator::onJobEvent(const JobEventType& event)
{
    if (event.type == JobEvent::STARTED) {
        auto plottingScreen = std::make_unique<screens::PlottingScreen>(_screensCtx);

        _router.pushScreen(std::move(plottingScreen));
        ESP_LOGI(TAG, "Navigated to PlottingScreen");
    }
}

} // namespace ui