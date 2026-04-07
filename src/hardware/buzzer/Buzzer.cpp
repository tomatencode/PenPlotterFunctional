#include "Buzzer.hpp"

Buzzer::Buzzer(uint8_t pin, uint8_t channel)
    : _pin(pin), _channel(channel), _active(false),
      _frequency(1000), _endTime(0),
      _melodyIndex(0) {}

void Buzzer::begin() {
    pinMode(_pin, OUTPUT);
    ledcSetup(_channel, 1000, 8); // initial 1 kHz, 8-bit
    ledcAttachPin(_pin, _channel);
    off();
}

void Buzzer::on(uint16_t frequency) {
    _frequency = frequency;
    ledcWriteTone(_channel, _frequency);
    _active = true;
    _endTime = 0;
    _melody.clear();
}

void Buzzer::off() {
    ledcWriteTone(_channel, 0);
    _active = false;
    _endTime = 0;
    _melody.clear();
}

void Buzzer::beep(uint16_t frequency, uint16_t duration) {
    _frequency = frequency;
    ledcWriteTone(_channel, _frequency);
    _active = true;
    _endTime = millis() + duration;
    _melody.clear();
}

void Buzzer::playMelody(const Melody& melody) {
    if (melody.empty()) return;

    _melody = melody;
    _melodyIndex = 0;
    _frequency = _melody[0].frequency;

    if (_frequency == 0) ledcWriteTone(_channel, 0);
    else ledcWriteTone(_channel, _frequency);

    _active = true;
    _endTime = millis() + _melody[0].duration;
}

void Buzzer::update() {
    if (!_active) return;

    unsigned long now = millis();

    if (!_melody.empty() && now >= _endTime) {
        _melodyIndex++;
        if (_melodyIndex >= _melody.size()) {
            off();
            return;
        }

        _frequency = _melody[_melodyIndex].frequency;

        if (_frequency == 0) ledcWriteTone(_channel, 0); // pause
        else ledcWriteTone(_channel, _frequency);

        _endTime = now + _melody[_melodyIndex].duration;
    } else if (_melody.empty() && _endTime != 0 && now >= _endTime) {
        off();
    }
}