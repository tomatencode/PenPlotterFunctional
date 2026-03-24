#pragma once

#include "jobManager/JobManager.hpp"
#include "jobManager/JobObserver.hpp"

#include "systemServices/MotionStateManager.hpp"

#include "hardware/display/LcdDisplay.hpp"
#include "hardware/rotaryEncoder/RotaryEncoder.hpp"
#include "hardware/buzzer/Buzzer.hpp"

#include "framework/render/Render.hpp"
#include "framework/input/InputState.hpp"
#include "framework/router/Router.hpp"

namespace ui {

class UiManager : public JobObserver
{
public:
    UiManager(JobManager& jobManager,MotionStateManager& ms, LcdDisplay& display, RotaryEncoder& encoder, Buzzer& buzzer);

    void init();

    void update();

    // JobObserver implementation
    void onJobEvent(const JobStatusUpdate& update) override;

private:
    // Core UI components
    Router _router;
    Renderer _renderer;
    Router* _currentRouter = nullptr;  // Cached reference to router for navigation

    // References to shared services (not owned by UI)
    JobManager& _jobManager;
    MotionStateManager& _ms;

    // References to hardware interfaces (not owned by UI)
    LcdDisplay& _display;
    RotaryEncoder& _encoder;
    Buzzer& _buzzer;

    // abstracted input reading to keep update() clean and focused on UI logic
    InputState readInputs();

    unsigned long _lastUpdateTime = 0;  // For non-blocking timing
};

} // namespace ui