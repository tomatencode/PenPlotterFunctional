#include <Arduino.h>
#include <functional>

struct StepperConfig {
    uint8_t stepPin;
    uint8_t dirPin;
};

using StepperFunc = std::function<void(bool direction)>;

StepperFunc create_stepper(StepperConfig config) {
    return [config](bool direction) -> void {
        digitalWrite(config.dirPin, direction ? HIGH : LOW);
        digitalWrite(config.stepPin, HIGH);
        delayMicroseconds(2);
        digitalWrite(config.stepPin, LOW);
    };
}