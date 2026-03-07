#include "TMC2209Driver.hpp"

TMC2209Driver::TMC2209Driver(TMC2209Stepper& driver)
    : _driver(driver) {}

void TMC2209Driver::begin() {
    _driver.begin();
}

void TMC2209Driver::setMicrosteps(uint16_t ms) {
    _driver.microsteps(ms);
    _microsteps = ms;
}

uint16_t TMC2209Driver::getMicrosteps() {
    if (_microsteps == 0) {
        // If microsteps haven't been set through this interface, read from the driver
        _microsteps = _driver.microsteps();
    }
    return _microsteps;
}

void TMC2209Driver::setCurrent(uint16_t mA) {
    _driver.rms_current(mA);
}

void TMC2209Driver::setStallGuardThreshold(uint8_t threshold) {
    _driver.SGTHRS(threshold);
}

uint32_t TMC2209Driver::getStallGuardResult() {
    return _driver.SG_RESULT();
}

void TMC2209Driver::setSpeed(float stps_per_s) {
    _driver.VACTUAL(stps_per_s);
}