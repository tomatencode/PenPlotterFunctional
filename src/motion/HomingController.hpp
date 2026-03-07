#ifndef HomingController_HPP
#define HomingController_HPP

#include "hardware/axis/StepperAxis.hpp"

class HomingController {
public:
    HomingController(StepperAxis& axisA, StepperAxis& axisB, MotorDriver& driverA, MotorDriver& driverB, float speed_stps_per_s, float stallGuard_threshold, float sgCheckInterval_ms, uint16_t consecutiveStallChecks, uint16_t sgStartTimeout_ms);

    void home();
private:
    StepperAxis& _axisA;
    StepperAxis& _axisB;
    MotorDriver& _driverA;
    MotorDriver& _driverB;
    float _speed_stps_per_s;
    float _stallGuard_threshold;
    float _sgCheckInterval_ms;
    uint16_t _consecutiveStallChecks;
    uint16_t _sgStartTimeout_ms;

    void moveToLimit(bool Afw, bool Bfw);
};

#endif