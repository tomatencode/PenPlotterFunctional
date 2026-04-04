#pragma once

#include <Arduino.h>
#include "settings/RuntimeSettings.hpp"
#include "settings/SettingObserver.hpp"
#include "hardware/steppers/Stepper.hpp"
#include "hardware/drivers/TMC2209Driver.hpp"
#include "gcodeExecution/motionExecution/StepperAxis.hpp"
#include "gcodeExecution/motionExecution/CoreXYKinematics.hpp"
#include "gcodeExecution/motionExecution/MotionExecuter.hpp"
#include "gcodeExecution/motionExecution/BezierExecuter.hpp"
#include "gcodeExecution/pen/ServoPen.hpp"
#include "gcodeExecution/homing/HomingController.hpp"
#include "gcodeExecution/GCodeExecuter.hpp"
#include "rtos/MotionState.hpp"
#include "rtos/RtosQueue.hpp"
#include "rtos/GcodeMessage.hpp"


class PlottingController : public SettingObserver
{
public:
    PlottingController(MotionState& motionState, RtosQueue<GcodeMessage>& gcodeQueue, RuntimeSettings& runtimeSettings);

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
};