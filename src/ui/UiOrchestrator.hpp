#pragma once

#include <functional>

#include "jobController/JobController.hpp"
#include "jobController/JobObserver.hpp"

#include "InputMapper.hpp"

#include "systemServices/MotionState.hpp"
#include "storage/FileManager.hpp"

#include "hardware/display/LcdDisplay.hpp"
#include "hardware/rotaryEncoder/RotaryEncoder.hpp"
#include "hardware/buzzer/Buzzer.hpp"

#include "framework/render/Render.hpp"
#include "framework/input/InputState.hpp"
#include "framework/router/Router.hpp"

// Forward declarations
class SettingsRepository;

namespace ui {

class UiOrchestrator : public JobObserver
{
public:
    UiOrchestrator(Router& router,
        Renderer& renderer,
        InputMapper& inputMapper,
        JobController& jobController,
        FileManager& fileManager,
        MotionState& motionState,
        Buzzer& buzzer,
        std::function<bool()> wifiStatusProvider,
        SettingsRepository& settingsRepository
    )
    : _router(router),
      _renderer(renderer),
      _inputMapper(inputMapper),
      _jobController(jobController),
      _fileManager(fileManager),
      _motionState(motionState),
      _buzzer(buzzer),
      _wifiStatusProvider(wifiStatusProvider),
      _settingsRepository(settingsRepository)
    {}
    
    ~UiOrchestrator() {
        _jobController.unregisterObserver(this);
    }

    void init();

    void update();

    // JobObserver implementation
    void onJobEvent(const JobEventType& event) override;

private:
    Router& _router;
    Renderer& _renderer;
    JobController& _jobController;
    MotionState& _motionState;
    FileManager& _fileManager;
    InputMapper& _inputMapper;
    Buzzer& _buzzer;
    std::function<bool()> _wifiStatusProvider;
    SettingsRepository& _settingsRepository;


    unsigned long _lastUpdateTime = 0;  // For non-blocking timing
};

} // namespace ui