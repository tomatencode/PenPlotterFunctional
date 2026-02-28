#ifndef STEPPER_AXIS_HPP
#define STEPPER_AXIS_HPP

#include <Arduino.h>
#include "Stepper.hpp"
#include "MotorDriver.hpp"

class StepperAxis {
    public:
        StepperAxis(Stepper& stepper, MotorDriver& driver);

        void step(bool clockwise);
        
        void zero();
        float positionSteps() const;

        // helper to expose information without leaking the driver
        uint16_t microsteps() const;

    private:
        MotorDriver& _driver;
        Stepper& _stepper;
        float _positionSteps;
};
#endif