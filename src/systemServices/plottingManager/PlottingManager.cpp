#include "PlottingManager.hpp"

#include "config/pins.hpp"
#include "config/hardware_config.hpp"
#include "systemServices/FreeRtosQueue.hpp"
#include "systemServices/RuntimeSettings.hpp"
#include "systemServices/GcodeMessage.hpp"

PlottingManager::PlottingManager(MotionState& motionState, FreeRtosQueue<GcodeMessage>& gcodeQueue, RuntimeSettings& runtimeSettings)
    : _motionState(motionState),
    gcodeQueue(gcodeQueue),
    _runtimeSettings(runtimeSettings),
    _driverSerial(1),

    _rawDriverA(&_driverSerial, R_SENSE, 0),
    _rawDriverB(&_driverSerial, R_SENSE, 2),

    _driverA(_rawDriverA),
    _driverB(_rawDriverB),

    _stepA(STEP_PIN_A, DIR_PIN_A),
    _stepB(STEP_PIN_B, DIR_PIN_B),

    _axisA(_stepA, _driverA, true),
    _axisB(_stepB, _driverB, true),

    _pen(_penServo, runtimeSettings),

    _homingController(
        _axisA,
        _axisB,
        _driverA,
        _driverB,
        motionState,
        runtimeSettings
    ),

    _kinematics(STEPS_PER_MM),

    _bezierExecuter(_axisA, _axisB, _kinematics, motionState),

    _motionExecuter(_bezierExecuter, motionState, runtimeSettings),

    _gcodeExecuter(
        _motionExecuter,
        _pen,
        _homingController,
        runtimeSettings,
        motionState
    )
{
}

void PlottingManager::configureDriver(TMC2209Driver& driver)
{
    driver.begin();
    driver.setStallGuardThreshold(_runtimeSettings.stallguardThreshold());
    driver.setCurrent(_runtimeSettings.driverCurrent_mA());
    driver.setMicrosteps(_runtimeSettings.microsteps());
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

        _gcodeExecuter.executeLine(msg->line);
    }
    else
    {
        if (_motionState.getCommand() == MotionCommand::ABORT) {
            _pen.up();
        }
        _motionState.setState(MotionStateType::IDLE);
    }
}