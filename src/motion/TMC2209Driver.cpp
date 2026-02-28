#include "TMC2209Driver.hpp"


class TMC2209Driver : public MotorDriver {
public:
    TMC2209Driver(TMC2209Stepper& driver)
        : _driver(driver) {}

    void begin() override {
        _driver.begin();
    }

    void setMicrosteps(uint16_t ms) override {
        _driver.microsteps(ms);
        _microsteps = ms;
    }

    uint16_t getMicrosteps() override {
        if (_microsteps == 0) {
            // If microsteps haven't been set through this interface, read from the driver
            _microsteps = _driver.microsteps();
        }
        return _microsteps;
    }

    void setCurrent(uint16_t mA) override {
        _driver.rms_current(mA);
    }

private:
    TMC2209Stepper& _driver;
    uint16_t _microsteps;
};