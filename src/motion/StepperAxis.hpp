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
    private:
        Stepper& _stepper;
        float _positionSteps;
        MotorDriver& _driver;
};
#endif