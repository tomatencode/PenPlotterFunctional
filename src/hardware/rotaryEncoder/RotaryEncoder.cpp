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

long RotaryEncoder::getPosition()
{
    noInterrupts();
    long pos = _position;
    interrupts();
    return pos;
}

void RotaryEncoder::reset()
{
    noInterrupts();
    _stepAccumulator = 0;
    _position = 0;
    interrupts();
}

bool RotaryEncoder::buttonPressed()
{
    if (_buttonFlag) {
        _buttonFlag = false;
        return true;
    }
    return false;
}

void RotaryEncoder::updateEncoder()
{
    uint8_t newState = (digitalRead(_clk) << 1) | digitalRead(_dt);

    uint8_t index = (_state << 2) | newState;

    _stepAccumulator += transitionTable[index];

    // Emit a full detent step if accumulator reaches 4
    if (_stepAccumulator >= 4) {
        _position++;
        _stepAccumulator -= 4;
    } 
    else if (_stepAccumulator <= -4) {
        _position--;
        _stepAccumulator += 4; 
    }

    _state = newState;
}

void RotaryEncoder::handleButton()
{
    unsigned long now = millis();

    if (now - _lastButtonTime > _debounce) {

        // confirm the button is getting pressed
        if (digitalRead(_sw) == LOW) {
            _buttonFlag = true;
        }
        _lastButtonTime = now;
    }
}
void RotaryEncoder::isrEncoder()
{
    instance->updateEncoder();
}

void RotaryEncoder::isrButton()
{
    instance->handleButton();
}