#ifndef STEPPER_AXIS_HPP
#define STEPPER_AXIS_HPP

#include <Arduino.h>
#include "Stepper.hpp"
#include "MotorDriver.hpp"

class StepperAxis {
    public:
        StepperAxis(Stepper& stepper, MotorDriver& driver, bool flippedDirection);

        void step(bool clockwise);
        
        void setPositionSteps(double steps);
        double positionSteps() const;

        uint16_t microsteps() const;
    private:
        bool flippedDirection;
        MotorDriver& _driver;
        Stepper& _stepper;
        double _positionSteps;
};
#endif