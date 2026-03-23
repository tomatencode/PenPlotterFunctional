#pragma once

namespace ui {

struct InputState
{   
    int encoderDelta = 0;   // steps since last frame

    bool buttonPressed = false;
    bool buttonReleased = false;
    bool buttonDown = false;
};

} // namespace ui