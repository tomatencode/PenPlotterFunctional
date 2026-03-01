#include "ServoPen.hpp"

ServoPen::ServoPen(Servo& servo, int upPosition, int downPosition)
        : _servo(servo), _upPosition(upPosition), _downPosition(downPosition), _penDown(false) {};

void ServoPen::down() {
    _servo.write(_downPosition);
    _penDown = true;
}

void ServoPen::up() {
    _servo.write(_upPosition);
    _penDown = false;
}

bool ServoPen::isDown() const {
    return _penDown;
}