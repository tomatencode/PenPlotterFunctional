#pragma once

namespace ui {

struct InputState
{   
    int encoderDelta = 0;

    bool buttonPressed = false;
    bool buttonReleased = false;
    bool buttonDown = false;
};

} // namespace ui