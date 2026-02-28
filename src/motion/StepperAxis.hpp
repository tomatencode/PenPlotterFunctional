#ifndef AXIS_H
#define AXIS_H

#include <Arduino.h>
#include "Stepper.hpp"

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