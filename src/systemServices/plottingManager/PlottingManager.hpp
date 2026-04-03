#pragma once

#include <Arduino.h>
#include "gcode/GCodeParser.hpp"
#include "config/machine_config.hpp"
#include "hardware/steppers/Stepper.hpp"
#include "hardware/pen/ServoPen.hpp"
#include "hardware/drivers/TMC2209Driver.hpp"
#include "motion/StepperAxis.hpp"
#include "motion/CoreXYKinematics.hpp"
#include "motion/MotionSystem.hpp"
#include "motion/HomingController.hpp"
#include "systemServices/MotionState.hpp"
#include "systemServices/FreeRtosQueue.hpp"
#include "systemServices/GcodeMessage.hpp"


class PlottingManager
{
public:
    PlottingManager(MotionState& motionState, FreeRtosQueue<GcodeMessage>& gcodeQueue);

    void init();
    void update();

private:

    void configureDriver(TMC2209Driver& driver);

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

    MotionSystem _motionSystem;

    GCodeParser _gcodeParser;

    MotionState& _motionState;
    FreeRtosQueue<GcodeMessage>& gcodeQueue;
};