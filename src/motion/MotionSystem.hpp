#ifndef MotionSystem_H
#define MotionSystem_H

#include <cstdint>
#include "CoreXYKinematics.hpp"
#include "StepperAxis.hpp"

using StepCallback = void(*)(bool direction);


class MotionSystem {
    public:
        MotionSystem(StepperAxis& axisA, StepperAxis& axisB, CoreXYKinematics& kinematics);

        // Plan and execute a linear move directly using a callback
        void moveToXY(
            const XYPos& targetPos,
            float mm_per_s
        );
    private:
        StepperAxis& _axisA;
        StepperAxis& _axisB;
        CoreXYKinematics& _kinematics;
};


#endif