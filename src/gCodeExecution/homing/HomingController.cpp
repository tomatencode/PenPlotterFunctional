#include "HomingController.hpp"
#include "settings/RuntimeSettings.hpp"

// Constants for homing behavior
static constexpr uint32_t HOMING_TIMEOUT_US = 10000000UL;  // 10 seconds
static constexpr uint32_t SG_CHECK_INTERVAL_MS = 50;
static constexpr uint32_t SG_START_TIMEOUT_MS = 200;
static constexpr uint8_t SG_HISTORY_SIZE = 10;             // Number of SG readings to average
static constexpr uint32_t INTER_LIMIT_DELAY_MS = 500;
static constexpr uint16_t BACK_OFF_STEPS = 15;

HomingController::HomingController(StepperAxis& axisA, StepperAxis& axisB, MotorDriver& driverA, MotorDriver& driverB, MotionState& motionState, RuntimeSettings& runtimeSettings)
    : _axisA(axisA), _axisB(axisB), _driverA(driverA), _driverB(driverB), _motionState(motionState), _runtimeSettings(runtimeSettings) {}

// Handle pause/abort during movement; return true if homing should be aborted
bool HomingController::checkPauseAbort() {
    if (_motionState.getCommand() == MotionCommand::PAUSE) {
        _driverA.setSpeed(0);
        _driverB.setSpeed(0);
        _motionState.setState(MotionStateType::PAUSED);

        Serial.println("Homing paused");
        while (_motionState.getCommand() == MotionCommand::PAUSE) {
            yield();
        }
        Serial.println("Homing resumed");
        _motionState.setState(MotionStateType::RUNNING);
        return false;  // Continue homing
    } else if (_motionState.getCommand() == MotionCommand::ABORT) {
        Serial.println("Homing aborted");
        _driverA.setSpeed(0);
        _driverB.setSpeed(0);
        return true;  // Abort homing
    }
    return false;  // Continue normally
}

// Move stepper axes toward limit switches until stallguard detects stall
void HomingController::moveToLimit(bool Afw, bool Bfw) {
    float speed_stps_per_s = _runtimeSettings.homingSpeed_stp_per_s();
    float stallGuard_threshold = _runtimeSettings.stallguardThreshold();
    
    if (speed_stps_per_s <= 0.0f) {
        Serial.println("ERROR: Invalid homing speed");
        return;
    }
    
    if (_driverA.getMicrosteps() != _driverB.getMicrosteps()) {
        Serial.println("ERROR: Drivers have different microstep settings");
        return;
    }

    uint32_t sgCheckInterval_us = SG_CHECK_INTERVAL_MS * 1000UL;
    uint32_t last_SGcheck_time = micros() + SG_START_TIMEOUT_MS * 1000UL;
    uint32_t start_time = micros();

    // Circular buffer for stallguard history
    int sgHistory[SG_HISTORY_SIZE] = {0};
    uint8_t sgHistoryIndex = 0;
    bool bufferFilled = false;

    // Set speeds with direction
    double speedA = speed_stps_per_s * _driverA.getMicrosteps() * (Afw ? 1 : -1);
    double speedB = speed_stps_per_s * _driverB.getMicrosteps() * (Bfw ? 1 : -1);
    _driverA.setSpeed(speedA);
    _driverB.setSpeed(speedB);

    while (true) {
        // Wait for next stallguard check interval
        while ((uint32_t)(micros() - last_SGcheck_time) < sgCheckInterval_us) {
            yield();
        }
        last_SGcheck_time = micros();

        // Check for pause/abort
        if (checkPauseAbort()) {
            return;
        }

        // Get current stallguard values (take worst of both axes)
        int stallGuardA = _driverA.getStallGuardResult();
        int stallGuardB = _driverB.getStallGuardResult();
        int stallGuardMin = (stallGuardA < stallGuardB) ? stallGuardA : stallGuardB;
        
        // Add to circular buffer
        sgHistory[sgHistoryIndex] = stallGuardMin;
        sgHistoryIndex = (sgHistoryIndex + 1) % SG_HISTORY_SIZE;
        
        // Mark buffer as filled after first full cycle
        if (sgHistoryIndex == 0) {
            bufferFilled = true;
        }

        // Only check average after buffer is filled
        if (bufferFilled) {
            int sum = 0;
            for (uint8_t i = 0; i < SG_HISTORY_SIZE; i++) {
                sum += sgHistory[i];
            }
            int average = sum / SG_HISTORY_SIZE;
            
            if (average < stallGuard_threshold) {
                Serial.print("Stall detected: avg SG = ");
                Serial.println(average);
                break;  // Stall detected, stop moving
            }
        }

        // Safety timeout
        if ((uint32_t)(micros() - start_time) > HOMING_TIMEOUT_US) {
            Serial.println("WARNING: Homing timeout, limit not found");
            break;
        }
    }

    _driverA.setSpeed(0);
    _driverB.setSpeed(0);
}

// Full homing sequence: move to limits, back off, zero position
void HomingController::home() {
    if (_axisA.microsteps() != _axisB.microsteps()) {
        Serial.println("ERROR: Axes have different microstep settings");
        return;
    }

    // Move to X limit (both axes negative)
    moveToLimit(false, false);
    if (_motionState.getCommand() == MotionCommand::ABORT) return;

    // Interruptible delay before moving to Y limit
    uint32_t delayStart = millis();
    while ((uint32_t)(millis() - delayStart) < INTER_LIMIT_DELAY_MS) {
        if (checkPauseAbort()) return;
        yield();
    }

    // Move to Y limit (B axis positive)
    moveToLimit(false, true);
    if (_motionState.getCommand() == MotionCommand::ABORT) return;

    // Back off from limits
    double microsteps_per_s = _runtimeSettings.homingSpeed_stp_per_s() * _axisA.microsteps();
    uint16_t stepInterval_us = 1000000UL / microsteps_per_s;

    for (uint16_t i = 0; i < BACK_OFF_STEPS * _axisA.microsteps(); i++) {
        if (checkPauseAbort()) return;
        
        _axisA.step(true);
        _axisB.step(true);
        delayMicroseconds(stepInterval_us);
    }

    // Zero both axes
    _axisA.setPositionSteps(0);
    _axisB.setPositionSteps(0);
}