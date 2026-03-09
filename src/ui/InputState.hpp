#ifndef INPUT_STATE_HPP
#define INPUT_STATE_HPP

struct ButtonState {
    bool buttonPressed = false;
    bool buttonReleased = false;
    bool buttonDown = false;
};

struct InputState
{
    int encoderDelta = 0;   // steps since last frame

    ButtonState buttonState;  // current button state
};

#endif