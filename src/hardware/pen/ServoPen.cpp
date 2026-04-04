#include "ServoPen.hpp"
#include "systemServices/RuntimeSettings.hpp"

ServoPen::ServoPen(Servo& servo, RuntimeSettings& runtimeSettings)
        : _servo(servo), _runtimeSettings(runtimeSettings), _penDown(false) {};

void ServoPen::down() {
    _servo.write((int)_runtimeSettings.penDownAngle_deg());
    delay(100); // Short delay to allow servo to move down before marking pen as down
    _penDown = true;
}

void ServoPen::up() {
    _servo.write((int)_runtimeSettings.penUpAngle_deg());
    delay(100); // Short delay to allow servo to move up before marking pen as up
    _penDown = false;
}

bool ServoPen::isDown() const {
    return _penDown;
}