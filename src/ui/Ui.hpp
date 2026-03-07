#ifndef UI_HPP
#define UI_HPP

#include "hardware/display/TextDisplay.hpp"
#include "hardware/rotaryEncoder/RotaryEncoder.hpp"
#include "systemServices/shared/SharedData.hpp" // for telemetry and MotionState

class UI
{
public:
    UI(TextDisplay& display, RotaryEncoder& encoder);
    void update();

private:
    TextDisplay& _display;
    RotaryEncoder& _encoder;
};

#endif