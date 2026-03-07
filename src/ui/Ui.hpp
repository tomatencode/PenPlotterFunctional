#ifndef UI_HPP
#define UI_HPP

#include "hardware/display/LcdDisplay.hpp"
#include "hardware/rotaryEncoder/RotaryEncoder.hpp"
#include "hardware/buzzer/Buzzer.hpp"
#include "systemServices/shared/SharedData.hpp"

class UI
{
public:
    UI(LcdDisplay& display, RotaryEncoder& encoder, Buzzer& buzzer);
    void update();

private:
    LcdDisplay& _display;
    RotaryEncoder& _encoder;
    Buzzer& _buzzer;
};

#endif