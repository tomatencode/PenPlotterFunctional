#ifndef UI_HPP
#define UI_HPP

#include "hardware/display/TextDisplay.hpp"
#include "systemServices/shared/SharedData.hpp" // for telemetry and MotionState

class UI
{
public:
    UI(TextDisplay& display);
    void update();

private:
    TextDisplay& _display;
};

#endif