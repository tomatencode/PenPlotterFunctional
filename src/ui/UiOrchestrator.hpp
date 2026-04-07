#pragma once

#include "hardware/buzzer/Buzzer.hpp"

#include "framework/renderer/Renderer.hpp"
#include "framework/input/InputState.hpp"
#include "framework/router/Router.hpp"
#include "screens/ScreensContext.hpp"

#include "jobController/JobObserver.hpp"

#include "InputMapper.hpp"

namespace ui {

class UiOrchestrator : public JobObserver
{
public:
    UiOrchestrator(JobController& jobController,
        Router& router,
        Renderer& renderer,
        InputMapper& inputMapper,
        Buzzer& buzzer,
        ScreensContext screensCtx
    )
    : _jobController(jobController),
      _router(router),
      _renderer(renderer),
      _inputMapper(inputMapper),
      _buzzer(buzzer),
      _screensCtx(screensCtx)
    {}
    
    ~UiOrchestrator();

    void init();

    void update();

    // JobObserver implementation
    void onJobEvent(const JobEventType& event) override;

private:
    JobController& _jobController;
    Router& _router;
    Renderer& _renderer;
    InputMapper& _inputMapper;
    Buzzer& _buzzer;
    ScreensContext _screensCtx;

    unsigned long _lastUpdateTime = 0;  // For non-blocking timing
};

} // namespace ui