#ifndef TMC2209_DRIVER_HPP
#define TMC2209_DRIVER_HPP

#include <TMCStepper.h>
#include "MotorDriver.hpp"

class TMC2209Driver : public MotorDriver {
public:
    TMC2209Driver(TMC2209Stepper& driver);

    void begin() override;
    void setMicrosteps(uint16_t ms) override;
    uint16_t getMicrosteps() override;
    void setCurrent(uint16_t mA) override;

    void setStallGuardThreshold(uint8_t threshold) override;
    uint32_t getStallGuardResult() override;

    void setSpeed(float stps_per_s) override;
private:
    TMC2209Stepper& _driver;
    uint16_t _microsteps{0};
};

#endif