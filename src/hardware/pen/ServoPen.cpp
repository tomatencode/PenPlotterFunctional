#include "ServoPen.hpp"

ServoPen::ServoPen(Servo& servo, int upPosition, int downPosition)
        : _servo(servo), _upPosition(upPosition), _downPosition(downPosition), _penDown(false) {};

void ServoPen::down() {
    _servo.write(_downPosition);
    delay(100); // Short delay to allow servo to move down before marking pen as down
    _penDown = true;
}

void ServoPen::up() {
    _servo.write(_upPosition);
    delay(100); // Short delay to allow servo to move up before marking pen as up
    _penDown = false;
}

bool ServoPen::isDown() const {
    return _penDown;
}