#ifndef ROTARY_ENCODER_HPP
#define ROTARY_ENCODER_HPP

#include <Arduino.h>

class RotaryEncoder {
public:
    RotaryEncoder(uint8_t dt, uint8_t clk, uint8_t sw, uint16_t debounce);

    void begin();

    long getPosition();
    void reset();

    bool buttonPressed();

private:
    uint8_t _dt;
    uint8_t _clk;
    uint8_t _sw;

    volatile long _position = 0;
    volatile uint8_t _state = 0;
    volatile int8_t _stepAccumulator = 0;

    volatile bool _buttonFlag = false;

    uint16_t _debounce;
    volatile unsigned long _lastButtonTime = 0;

    void updateEncoder();
    void handleButton();

    static void isrEncoder();
    static void isrButton();

    static RotaryEncoder* instance;
};

#endif