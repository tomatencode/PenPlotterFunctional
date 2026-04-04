#pragma once

#include <ESP32Servo.h>
#include "Pen.hpp"

// Forward declaration
class RuntimeSettings;

class ServoPen : public Pen {
public:
    ServoPen(Servo& servo, RuntimeSettings& runtimeSettings);

    void down() override;
    void up() override;
    bool isDown() const override;

private:
    Servo& _servo;
    RuntimeSettings& _runtimeSettings;
    bool _penDown;
};