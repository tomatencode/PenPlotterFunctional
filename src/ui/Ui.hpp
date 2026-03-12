#pragma once

#include "hardware/display/LcdDisplay.hpp"
#include "hardware/rotaryEncoder/RotaryEncoder.hpp"
#include "hardware/buzzer/Buzzer.hpp"
#include "systemServices/shared/SharedData.hpp"

#include "render/Render.hpp"
#include "InputState.hpp"
#include "router/Router.hpp"

class UI
{
public:
    UI(LcdDisplay& display, RotaryEncoder& encoder, Buzzer& buzzer);

    void init();

    void update();

private:
    InputState readInputs();

    Router _router;
    Renderer _renderer;

    LcdDisplay& _display;
    RotaryEncoder& _encoder;
    Buzzer& _buzzer;
};