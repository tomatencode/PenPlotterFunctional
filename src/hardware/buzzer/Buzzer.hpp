#ifndef BUZZER_HPP
#define BUZZER_HPP

#include <Arduino.h>

class Buzzer {
public:
    // Nested Melody type
    struct Melody {
        static constexpr size_t MAX_NOTES = 32;
        uint16_t notes[MAX_NOTES];      // Hz, 0 = pause
        uint16_t durations[MAX_NOTES];  // ms
        size_t length = 0;

        Melody() = default;

        template <size_t N>
        Melody(const uint16_t (&n)[N], const uint16_t (&d)[N]) {
            length = (N > MAX_NOTES) ? MAX_NOTES : N;
            for (size_t i = 0; i < length; i++) {
                notes[i] = n[i];
                durations[i] = d[i];
            }
        }
    };

    Buzzer(uint8_t pin, uint8_t channel = 5);

    void begin();
    void on(uint16_t frequency = 1000); 
    void off();
    void beep(uint16_t frequency, uint16_t duration);

    void playMelody(const Melody& melody);
    void update();

private:
    uint8_t _pin;
    uint8_t _channel;
    bool _active;
    uint16_t _frequency;
    unsigned long _endTime;

    uint16_t _melodyNotes[Melody::MAX_NOTES];
    uint16_t _melodyDurations[Melody::MAX_NOTES];
    size_t _melodyLength;
    size_t _melodyIndex;
};

#endif