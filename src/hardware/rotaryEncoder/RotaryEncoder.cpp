#include "RotaryEncoder.hpp"

RotaryEncoder* RotaryEncoder::instance = nullptr;

static const int8_t transitionTable[16] = {
     0, -1,  1,  0,
     1,  0,  0, -1,
    -1,  0,  0,  1,
     0,  1, -1,  0
};

RotaryEncoder::RotaryEncoder(uint8_t dt, uint8_t clk, uint8_t sw, uint16_t debounce)
{
    _dt = dt;
    _clk = clk;
    _sw = sw;
    _debounce = debounce;
}

void RotaryEncoder::begin()
{
    instance = this;

    pinMode(_dt, INPUT_PULLUP);
    pinMode(_clk, INPUT_PULLUP);
    pinMode(_sw, INPUT_PULLUP);

    _state = (digitalRead(_clk) << 1) | digitalRead(_dt);

    attachInterrupt(digitalPinToInterrupt(_dt), isrEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(_clk), isrEncoder, CHANGE);

    attachInterrupt(digitalPinToInterrupt(_sw), isrButton, CHANGE);
}

int RotaryEncoder::getPositionDelta()
{
    noInterrupts();
    int pos_delta = _position_delta;
    _position_delta = 0; // reset after reading
    interrupts();
    return pos_delta;
}

void RotaryEncoder::reset()
{
    noInterrupts();
    _stepAccumulator = 0;
    _position_delta = 0;
    interrupts();
}

bool RotaryEncoder::buttonPressed()
{
    if (_buttonPrsFlag) {
        _buttonPrsFlag = false;
        return true;
    }
    return false;
}

bool RotaryEncoder::buttonReleased()
{
    if (_buttonRelFlag) {
        _buttonRelFlag = false;
        return true;
    }
    return false;
}

bool RotaryEncoder::buttonDown()
{
    noInterrupts();
    bool isDown = digitalRead(_sw) == LOW;
    interrupts();
    return isDown;
}

void RotaryEncoder::updateEncoder()
{
    uint8_t newState = (digitalRead(_clk) << 1) | digitalRead(_dt);

    uint8_t index = (_state << 2) | newState;

    _stepAccumulator += transitionTable[index];

    // Emit a full detent step if accumulator reaches 4
    if (_stepAccumulator >= 4) {
        _position_delta++;
        _stepAccumulator -= 4;
    } 
    else if (_stepAccumulator <= -4) {
        _position_delta--;
        _stepAccumulator += 4; 
    }

    _state = newState;
}

void RotaryEncoder::handleButton()
{
    bool buttonState = digitalRead(_sw);

    if (buttonState == _lastButtonState) {
        return; // No change in button state, ignore
    }

    unsigned long now = millis();

    // Handle button press with debounce
    if (now - _ButtonPrsDebounceTime > _debounce && now - _ButtonRelDebounceTime > _debounce) {
        // often a release can trigger a press, so we want to make sure both debounce times have passed before registering a new press
        // this also means that if the button is repressed very quickly, it will be missed but that is a reasonable tradeoff

        // confirm the button is getting pressed
        if (buttonState == LOW) {
            _buttonPrsFlag = true;
            _ButtonPrsDebounceTime = now;
        }
    }

    // Handle button release with debounce
    if (now - _ButtonRelDebounceTime > _debounce) {

        // confirm the button is getting released
        if (buttonState == HIGH) {
            _buttonRelFlag = true;
            _ButtonRelDebounceTime = now;
        }
    }

    _lastButtonState = buttonState;
}

void RotaryEncoder::isrEncoder()
{
    instance->updateEncoder();
}

void RotaryEncoder::isrButton()
{
    instance->handleButton();
}