#pragma once

#include <Arduino.h>
#include "settings/RuntimeSettings.hpp"
#include "settings/SettingObserver.hpp"
#include "hardware/steppers/Stepper.hpp"
#include "hardware/drivers/TMC2209Driver.hpp"
#include "gCodeExecution/motionExecution/StepperAxis.hpp"
#include "gCodeExecution/motionExecution/CoreXYKinematics.hpp"
#include "gCodeExecution/motionExecution/MotionExecuter.hpp"
#include "gCodeExecution/motionExecution/BezierExecuter.hpp"
#include "gCodeExecution/pen/ServoPen.hpp"
#include "gCodeExecution/homing/HomingController.hpp"
#include "gCodeExecution/GCodeExecuter.hpp"
#include "systemServices/MotionState.hpp"
#include "systemServices/FreeRtosQueue.hpp"
#include "systemServices/GcodeMessage.hpp"


class PlottingManager : public SettingObserver
{
public:
    PlottingManager(MotionState& motionState, FreeRtosQueue<GcodeMessage>& gcodeQueue, RuntimeSettings& runtimeSettings);

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
    FreeRtosQueue<GcodeMessage>& gcodeQueue;
    RuntimeSettings& _runtimeSettings;
};