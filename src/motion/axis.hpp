#ifndef AXIS_H
#define AXIS_H

#include <Arduino.h>
#include "stepper.hpp"

class Axis {
    public:
        Axis(Stepper& stepper);

        void step(int direction);
        
        void zero();
        int32_t positionSteps() const;
    private:
        Stepper& _stepper;
        int32_t _positionSteps = 0;
};
#endif