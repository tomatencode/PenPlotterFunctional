#ifndef STEPPER_AXIS_HPP
#define STEPPER_AXIS_HPP

#include <Arduino.h>
#include "Stepper.hpp"
#include "MotorDriver.hpp"

class StepperAxis {
    public:
        StepperAxis(Stepper& stepper, MotorDriver& driver);

        void step(bool clockwise);
        
        void setPositionSteps(float steps);
        float positionSteps() const;

        uint16_t microsteps() const;
    private:
        MotorDriver& _driver;
        Stepper& _stepper;
        float _positionSteps;
};
#endif