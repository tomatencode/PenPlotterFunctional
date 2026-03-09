#include "ButtonState.hpp"

struct InputState
{
    int encoderDelta = 0;   // steps since last frame

    ButtonState buttonState;  // current button state
};