#include "Buzzer.hpp"

Buzzer::Buzzer(uint8_t pin, uint8_t channel)
    : _pin(pin), _channel(channel), _active(false),
      _frequency(1000), _endTime(0),
      _melodyLength(0), _melodyIndex(0) {}

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
    _melodyLength = 0;
}

void Buzzer::off() {
    ledcWriteTone(_channel, 0);
    _active = false;
    _endTime = 0;
    _melodyLength = 0;
}

void Buzzer::beep(uint16_t frequency, uint16_t duration) {
    _frequency = frequency;
    ledcWriteTone(_channel, _frequency);
    _active = true;
    _endTime = millis() + duration;
    _melodyLength = 0;
}

void Buzzer::playMelody(const Melody& melody) {
    if (melody.length == 0) return;

    _melodyLength = melody.length;

    for (size_t i = 0; i < _melodyLength; i++) {
        _melodyNotes[i] = melody.notes[i];
        _melodyDurations[i] = melody.durations[i];
    }

    _melodyIndex = 0;
    _frequency = _melodyNotes[0];

    if (_frequency == 0) ledcWriteTone(_channel, 0); 
    else ledcWriteTone(_channel, _frequency);

    _active = true;
    _endTime = millis() + _melodyDurations[0];
}

void Buzzer::update() {
    if (!_active) return;

    unsigned long now = millis();

    if (_melodyLength > 0 && now >= _endTime) {
        _melodyIndex++;
        if (_melodyIndex >= _melodyLength) {
            off();
            return;
        }

        _frequency = _melodyNotes[_melodyIndex];

        if (_frequency == 0) ledcWriteTone(_channel, 0); // pause
        else ledcWriteTone(_channel, _frequency);

        _endTime = now + _melodyDurations[_melodyIndex];
    } else if (_melodyLength == 0 && _endTime != 0 && now >= _endTime) {
        off();
    }
}