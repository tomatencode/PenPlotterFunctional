#include "ServoPen.hpp"

ServoPen::ServoPen(Servo& servo, int upPosition, int downPosition)
        : _servo(servo), _upPosition(upPosition), _downPosition(downPosition) {};

void ServoPen::down() {
    _servo.write(_downPosition);
}

void ServoPen::up() {
    _servo.write(_upPosition);
}