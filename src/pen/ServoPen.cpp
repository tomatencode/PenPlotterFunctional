#include "ServoPen.hpp"
#include "settings/RuntimeSettings.hpp"

ServoPen::ServoPen(Servo& servo, SettingPersistence& settingPersistence, RuntimeSettings& runtimeSettings)
    : SettingObserver({Setting::PenUpAngle, Setting::PenDownAngle}),
    _servo(servo),
    _settingPersistence(settingPersistence),
    _runtimeSettings(runtimeSettings),
    _penDown(false)
{
    _settingPersistence.registerObserver(this);
};

void ServoPen::init() {
    up();
}

ServoPen::~ServoPen() {
    _settingPersistence.unregisterObserver(this);
}

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

void ServoPen::onRelevantSettingsChanged() {
    // If pen is currently down, we need to update the servo position to reflect new angles
    if (_penDown) {
        down();
    } else {
        up();
    }
}