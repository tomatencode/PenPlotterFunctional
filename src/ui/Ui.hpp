#ifndef UI_HPP
#define UI_HPP

#include "hardware/display/LcdDisplay.hpp"
#include "hardware/rotaryEncoder/RotaryEncoder.hpp"
#include "systemServices/shared/SharedData.hpp" // for telemetry and MotionState

class UI
{
public:
    UI(LcdDisplay& display, RotaryEncoder& encoder);
    void update();

private:
    LcdDisplay& _display;
    RotaryEncoder& _encoder;
};

#endif