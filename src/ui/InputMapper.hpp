#pragma once

#include "framework/input/InputState.hpp"

#include "hardware/display/LcdDisplay.hpp"
#include "hardware/rotaryEncoder/RotaryEncoder.hpp"

namespace ui {

class InputMapper
{
public:
    InputMapper(RotaryEncoder& encoder)
    : _encoder(encoder)
    {}

    InputState mapInputs() {
        InputState state;

        state.encoderDelta = _encoder.getPositionDelta();
        state.buttonPressed = _encoder.buttonPressed();
        state.buttonReleased = _encoder.buttonReleased();
        state.buttonDown = _encoder.buttonDown();

        return state;
    }

private:
    RotaryEncoder& _encoder;
};

} // namespace ui