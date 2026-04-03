#include "PlottingManager.hpp"

#include "config/pins.hpp"
#include "systemServices/FreeRtosQueue.hpp"
#include "systemServices/GcodeMessage.hpp"

PlottingManager::PlottingManager(MotionState& motionState, FreeRtosQueue<GcodeMessage>& gcodeQueue)
    : _motionState(motionState),
    gcodeQueue(gcodeQueue),
    _driverSerial(1),

    _rawDriverA(&_driverSerial, R_SENSE, 0),
    _rawDriverB(&_driverSerial, R_SENSE, 2),

    _driverA(_rawDriverA),
    _driverB(_rawDriverB),

    _stepA(STEP_PIN_A, DIR_PIN_A),
    _stepB(STEP_PIN_B, DIR_PIN_B),

    _axisA(_stepA, _driverA, true),
    _axisB(_stepB, _driverB, true),

    _pen(_penServo, PEN_UP_ANGLE, PEN_DOWN_ANGLE),

    _homingController(
        _axisA,
        _axisB,
        _driverA,
        _driverB,
        motionState,
        HOMING_SPEED_STPS_PER_S,
        HOMING_STALLGUARD_THRESHOLD,
        HOMING_SG_CHECK_INTERVAL_MS,
        HOMING_CONSECUTIVE_STALL_CHECKS,
        HOMING_SG_START_TIMEOUT_MS
    ),

    _kinematics(STEPS_PER_MM),

    _motionSystem(_axisA, _axisB, _kinematics, motionState, MIN_FEATURE_SIZE_MM),

    _gcodeParser(
        _motionSystem,
        _pen,
        _homingController,
        FEED_RATE_DRAW_MM_PER_S,
        FEED_RATE_TRAVEL_MM_PER_S,
        motionState
    )
{
}

void PlottingManager::configureDriver(TMC2209Driver& driver)
{
    driver.begin();
    driver.setStallGuardThreshold(HOMING_STALLGUARD_THRESHOLD);
    driver.setCurrent(CURRENT_MA);
    driver.setMicrosteps(MICROSTEPS);
}

void PlottingManager::init()
{
    pinMode(SERVO_PIN, OUTPUT);

    _penServo.attach(SERVO_PIN);
    _pen.up();

    pinMode(EN_PIN, OUTPUT);
    digitalWrite(EN_PIN, LOW);

    _driverSerial.begin(115200, SERIAL_8N1, DRIVER_RX_PIN, DRIVER_TX_PIN);

    configureDriver(_driverA);
    configureDriver(_driverB);

    Serial.println("Plotting manager initialized.");
}

void PlottingManager::update()
{
    auto msg = gcodeQueue.tryReceive(0);

    if (msg.has_value() && _motionState.getCommand() != MotionCommand::ABORT)
    {
        _motionState.setState(MotionStateType::RUNNING);

        Serial.print("Executing line: ");
        Serial.println(msg->line);

        _gcodeParser.executeLine(msg->line);
    }
    else
    {
        _motionState.setState(MotionStateType::IDLE);
    }
}