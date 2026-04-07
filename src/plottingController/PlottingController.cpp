#include "PlottingController.hpp"

#include "config/pins.hpp"
#include "config/hardware_config.hpp"
#include "rtos/RtosQueue.hpp"
#include "rtos/GcodeMessage.hpp"

PlottingController::PlottingController(MotionState& motionState, RtosQueue<GcodeMessage>& gcodeQueue, SettingPersistence& settingsPersistence, RuntimeSettings& runtimeSettings)
    : SettingObserver({Setting::DriverCurrent, Setting::Microsteps, Setting::StallguardThreshold}),
    
    _motionState(motionState),
    gcodeQueue(gcodeQueue),
    _settingPersistence(settingsPersistence),
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

    _pen(_penServo, settingsPersistence, runtimeSettings),

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
    _settingPersistence.registerObserver(this);
}

PlottingController::~PlottingController() {
    _settingPersistence.unregisterObserver(this);
}

void PlottingController::configureDriver(TMC2209Driver& driver)
{
    driver.setStallGuardThreshold(_runtimeSettings.stallguardThreshold());
    driver.setCurrent(_runtimeSettings.driverCurrent_mA());
    driver.setMicrosteps(_runtimeSettings.microsteps());
}

void PlottingController::init()
{
    pinMode(SERVO_PIN, OUTPUT);

    _penServo.attach(SERVO_PIN);
    _pen.init();

    pinMode(EN_PIN, OUTPUT);
    digitalWrite(EN_PIN, LOW);

    _driverSerial.begin(115200, SERIAL_8N1, DRIVER_RX_PIN, DRIVER_TX_PIN);

    _driverA.begin();
    configureDriver(_driverA);

    _driverB.begin();
    configureDriver(_driverB);

    Serial.println("Plotting manager initialized.");
}

void PlottingController::update()
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

    _pen.checkIfSettingsChanged();
    SettingObserver::checkIfSettingsChanged();
}

void PlottingController::onRelevantSettingsChanged() {
    // Update driver configurations based on the changed settings
    configureDriver(_driverA);
    configureDriver(_driverB);
}