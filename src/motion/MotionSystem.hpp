#ifndef MotionSystem_H
#define MotionSystem_H

#include <cstdint>
#include "CoreXYKinematics.hpp"
#include "StepperAxis.hpp"

using StepCallback = void(*)(bool direction);


class MotionSystem {
    public:
        MotionSystem(StepperAxis& axisA, StepperAxis& axisB, CoreXYKinematics& kinematics, float min_feature_size_mm = 1.0f);

        // Plan and execute a linear move directly using a callback
        void moveToXY(
            const XYPos& targetPos,
            float mm_per_s
        );
        void arcToXY(
            const XYPos& targetPos,
            const XYPos& centerPos,
            bool clockwise,
            float mm_per_s
        );

    private:
        StepperAxis& _axisA;
        StepperAxis& _axisB;
        CoreXYKinematics& _kinematics;
        float _min_feature_size_mm;
};


#endif