#include "HomingController.hpp"
#include "systemServices/RuntimeSettings.hpp"

HomingController::HomingController(StepperAxis& axisA, StepperAxis& axisB, MotorDriver& driverA, MotorDriver& driverB, MotionState& motionState, RuntimeSettings& runtimeSettings)
    : _axisA(axisA), _axisB(axisB), _driverA(driverA), _driverB(driverB), _motionState(motionState), _runtimeSettings(runtimeSettings) {}

void HomingController::moveToLimit(bool Afw, bool Bfw)
{
    float speed_stps_per_s = _runtimeSettings.homingSpeed();
    float stallGuard_threshold = _runtimeSettings.stallguardThreshold();
    
    if (speed_stps_per_s <= 0.0f) return;
    if (_driverA.getMicrosteps() != _driverB.getMicrosteps()) return;


    uint32_t sgCheckInterval_us = (uint32_t)(_sgCheckInterval_ms * 1000.0f);
    uint32_t last_SGcheck_time = micros() + _sgStartTimeout_ms * 1000UL; // Start checking for stallGuard after a timeout to allow movement to start
    
    uint32_t start_time = micros();

    uint8_t stallCount = 0;

    double speedA = speed_stps_per_s * _driverA.getMicrosteps() * (Afw ? 1 : -1);
    double speedB = speed_stps_per_s * _driverB.getMicrosteps() * (Bfw ? 1 : -1);

    _driverA.setSpeed(speedA);
    _driverB.setSpeed(speedB);

    uint32_t last_step_time = micros();

    while (true)
    {
        while ((uint32_t)(micros() - last_SGcheck_time) < sgCheckInterval_us) { yield(); }
        last_SGcheck_time = micros();

        // Check for pause/abort commands
        if (_motionState.getCommand() == MotionCommand::PAUSE) {

            _driverA.setSpeed(0);
            _driverB.setSpeed(0);
            _motionState.setState(MotionStateType::PAUSED);

            Serial.println("Homing paused");

            while (_motionState.getCommand() == MotionCommand::PAUSE)
            {
                yield(); // for watchdog (idk if this is necessary)
                Serial.println("Waiting for resume...");
            }

            Serial.println("Homing resumed");

            _driverA.setSpeed(speedA);
            _driverB.setSpeed(speedB);
            _motionState.setState(MotionStateType::RUNNING);
            last_SGcheck_time = micros(); // Reset to avoid huge elapsed time after pause
            start_time = micros(); // Reset timeout so pause duration isn't counted

        }
        else if (_motionState.getCommand() == MotionCommand::ABORT)
        {
            Serial.println("Homing aborted");
            _driverA.setSpeed(0);
            _driverB.setSpeed(0);
            return;
        }

        int stallGuardA = _driverA.getStallGuardResult();
        int stallGuardB = _driverB.getStallGuardResult();
        
        if (stallGuardA < stallGuard_threshold || stallGuardB < stallGuard_threshold)
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
    if (_motionState.getCommand() == MotionCommand::ABORT) return;

    // Interruptible 500ms delay
    uint32_t delayStart = millis();
    while ((uint32_t)(millis() - delayStart) < 500) {
        if (_motionState.getCommand() == MotionCommand::ABORT) return;
        if (_motionState.getCommand() == MotionCommand::PAUSE) {
            _motionState.setState(MotionStateType::PAUSED);
            while (_motionState.getCommand() == MotionCommand::PAUSE) { yield(); }
            _motionState.setState(MotionStateType::RUNNING);
        }
        yield();
    }

    moveToLimit(false, true); // Move to y limit
    if (_motionState.getCommand() == MotionCommand::ABORT) return;

    uint16_t stepInterval_us = 1000000UL / _runtimeSettings.homingSpeed() * _axisA.microsteps();

    for (int i = 0; i < 15 * _axisA.microsteps(); i++) {
        if (_motionState.getCommand() == MotionCommand::PAUSE) {
            _motionState.setState(MotionStateType::PAUSED);
            while (_motionState.getCommand() == MotionCommand::PAUSE) { yield(); }
            _motionState.setState(MotionStateType::RUNNING);
        }
        else if (_motionState.getCommand() == MotionCommand::ABORT) {
            return;
        }

        _axisA.step(true);
        _axisB.step(true);
        delayMicroseconds(stepInterval_us);
    }

    _axisA.setPositionSteps(0);
    _axisB.setPositionSteps(0);
}