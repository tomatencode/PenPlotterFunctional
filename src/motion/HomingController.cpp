#include "motion/HomingController.hpp"

HomingController::HomingController(StepperAxis& axisA, StepperAxis& axisB, MotorDriver& driverA, MotorDriver& driverB, float speed_stps_per_s, float stallGuard_threshold, float sgCheckInterval_ms, uint16_t consecutiveStallChecks, uint16_t sgStartTimeout_ms)
    : _axisA(axisA), _axisB(axisB), _driverA(driverA), _driverB(driverB), _speed_stps_per_s(speed_stps_per_s), _stallGuard_threshold(stallGuard_threshold), _sgCheckInterval_ms(sgCheckInterval_ms), _consecutiveStallChecks(consecutiveStallChecks), _sgStartTimeout_ms(sgStartTimeout_ms) {}

void HomingController::moveToLimit(bool Afw, bool Bfw)
{
    if (_speed_stps_per_s <= 0.0f) return;
    if (_driverA.getMicrosteps() != _driverB.getMicrosteps()) return;


    uint32_t sgCheckInterval_us = (uint32_t)(_sgCheckInterval_ms * 1000.0f);
    uint32_t last_SGcheck_time = micros() + _sgStartTimeout_ms * 1000UL; // Start checking for stallGuard after a timeout to allow movement to start
    
    uint32_t start_time = micros();

    uint8_t stallCount = 0;

    _driverA.setSpeed(_speed_stps_per_s * _driverA.getMicrosteps() * (Afw ? 1 : -1));
    _driverB.setSpeed(_speed_stps_per_s * _driverB.getMicrosteps() * (Bfw ? 1 : -1));

    while (true)
    {
        while ((uint32_t)(micros() - last_SGcheck_time) >= sgCheckInterval_us) { yield(); }

        last_SGcheck_time += sgCheckInterval_us;

        int stallGuardA = _driverA.getStallGuardResult();
        int stallGuardB = _driverB.getStallGuardResult();
        
        if (stallGuardA < _stallGuard_threshold || stallGuardB < _stallGuard_threshold)
        {
            stallCount++;
            if (stallCount >= _consecutiveStallChecks) break;
        }
        else
        {
            stallCount = 0;
            }

        // 10 second timeout safety
        if ((uint32_t)(micros() - start_time) > 10000000UL)
            break;
    }

    _driverA.setSpeed(0);
    _driverB.setSpeed(0);
}

void HomingController::home() {
    if (_axisA.microsteps() != _axisB.microsteps()) return;

    moveToLimit(false, false); // Move to x limit
    delay(500); // Short delay to ensure we're fully at the limit
    moveToLimit(false, true); // Move to y limit
    
    uint16_t stepInterval_us = 1000000UL / _speed_stps_per_s;

    for (int i = 0; i < 15 * _axisA.microsteps(); i++) {
        _axisA.step(true);
        _axisB.step(true);
        delayMicroseconds(stepInterval_us); // Short delay between steps to ensure movement
    }

    _axisA.setPositionSteps(0);
    _axisB.setPositionSteps(0);
}