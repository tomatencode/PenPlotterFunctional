#pragma once

#include <cstdint>
#include "CoreXYKinematics.hpp"
#include "hardware/axis/StepperAxis.hpp"

using StepCallback = void(*)(bool direction);


class MotionSystem {
    public:
        MotionSystem(StepperAxis& axisA, StepperAxis& axisB, CoreXYKinematics& kinematics, double min_feature_size_mm = 1.0);

        void moveToXY(
            const XYPos& targetPos,
            double mm_per_s
        );
        void arcToXY(
            const XYPos& targetPos,
            const XYPos& centerPos,
            bool clockwise,
            double mm_per_s
        );
        void quadraticBezierToXY(
            const XYPos& controlPoint,
            const XYPos& targetPos,
            float mm_per_s
        );
        void cubicBezierToXY(
            const XYPos& controlPoint1,
            const XYPos& controlPoint2,
            const XYPos& targetPos,
            float mm_per_s
        );

        XYPos getCurrentPos() const {
            MotorSteps steps = {_axisA.positionSteps(), _axisB.positionSteps()};
            return _kinematics.steps_to_mm(steps);
        }
    private:
        StepperAxis& _axisA;
        StepperAxis& _axisB;
        CoreXYKinematics& _kinematics;
        double _min_feature_size_mm;
};