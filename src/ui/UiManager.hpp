#pragma once

#include "hardware/display/LcdDisplay.hpp"
#include "hardware/rotaryEncoder/RotaryEncoder.hpp"
#include "hardware/buzzer/Buzzer.hpp"
#include "systemServices/shared/SharedData.hpp"

#include "framework/render/Render.hpp"
#include "framework/input/InputState.hpp"
#include "framework/router/Router.hpp"

namespace ui {

class UiManager
{
public:
    UiManager(LcdDisplay& display, RotaryEncoder& encoder, Buzzer& buzzer);

    void init();

    void update();

private:
    // abstracted input reading to keep update() clean and focused on UI logic
    InputState readInputs();

    // Core UI components
    Router _router;
    Renderer _renderer;

    // References to hardware interfaces (not owned by UI)
    LcdDisplay& _display;
    RotaryEncoder& _encoder;
    Buzzer& _buzzer;

    unsigned long _lastUpdateTime = 0;  // For non-blocking timing
};

} // namespace ui