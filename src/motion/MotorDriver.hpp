#ifndef MOTOR_DRIVER_HPP
#define MOTOR_DRIVER_HPP

#include <cstdint>

class MotorDriver {
public:
    virtual void begin() = 0;
    virtual void setMicrosteps(uint16_t ms) = 0;
    virtual uint16_t getMicrosteps() = 0;
    virtual void setCurrent(uint16_t mA) = 0;

    virtual ~MotorDriver() = default;
private:
    uint16_t _microsteps;
};

#endif