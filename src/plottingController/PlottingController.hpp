#pragma once

#include <Arduino.h>
#include "settings/RuntimeSettings.hpp"
#include "settings/SettingPersistence.hpp"
#include "settings/SettingObserver.hpp"
#include "hardware/steppers/Stepper.hpp"
#include "hardware/drivers/TMC2209Driver.hpp"
#include "motionExecution/StepperAxis.hpp"
#include "motionExecution/CoreXYKinematics.hpp"
#include "motionExecution/MotionExecuter.hpp"
#include "motionExecution/BezierExecuter.hpp"
#include "pen/ServoPen.hpp"
#include "homing/HomingController.hpp"
#include "gcode/GCodeExecuter.hpp"
#include "rtos/MotionState.hpp"
#include "rtos/RtosQueue.hpp"
#include "rtos/GcodeMessage.hpp"


class PlottingController : public SettingObserver
{
public:
    PlottingController(MotionState& motionState, RtosQueue<GcodeMessage>& gcodeQueue, SettingPersistence& settingsPersistence, RuntimeSettings& runtimeSettings);
    ~PlottingController();

    void init();
    void update();

private:

    void configureDriver(TMC2209Driver& driver);

    void onRelevantSettingsChanged() override;

    // UART
    HardwareSerial _driverSerial;

    // Servo
    Servo _penServo;

    // Raw drivers
    TMC2209Stepper _rawDriverA;
    TMC2209Stepper _rawDriverB;

    // Driver abstraction
    TMC2209Driver _driverA;
    TMC2209Driver _driverB;

    // Motion hardware
    Stepper _stepA;
    Stepper _stepB;

    StepperAxis _axisA;
    StepperAxis _axisB;

    ServoPen _pen;

    HomingController _homingController;

    CoreXYKinematics _kinematics;

    BezierExecuter _bezierExecuter;

    MotionExecuter _motionExecuter;

    GCodeExecuter _gcodeExecuter;

    MotionState& _motionState;
    RtosQueue<GcodeMessage>& gcodeQueue;
    RuntimeSettings& _runtimeSettings;
    SettingPersistence& _settingPersistence;
};