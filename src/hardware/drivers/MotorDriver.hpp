#pragma once
#include <cstdint>

class MotorDriver {
public:
    virtual void begin() = 0;
    virtual void setMicrosteps(uint16_t ms) = 0;
    virtual uint16_t getMicrosteps() = 0;
    virtual void setCurrent(uint16_t mA) = 0;

    virtual void setStallGuardThreshold(uint8_t threshold) = 0; // Optional, can be no-op for drivers without stallGuard
    virtual uint32_t getStallGuardResult() = 0; // Optional, can return 0 for drivers without stallGuard

    virtual void setSpeed(float stps_per_s) = 0;

    virtual ~MotorDriver() = default;
};