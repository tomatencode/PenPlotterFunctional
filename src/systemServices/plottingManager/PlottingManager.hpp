#pragma once

#include <Arduino.h>
#include "gcode/GCodeParser.hpp"
#include "config/machine_config.hpp"
#include "hardware/steppers/Stepper.hpp"
#include "hardware/pen/ServoPen.hpp"
#include "hardware/axis/StepperAxis.hpp"
#include "hardware/drivers/TMC2209Driver.hpp"
#include "motion/CoreXYKinematics.hpp"
#include "motion/MotionSystem.hpp"
#include "motion/HomingController.hpp"
#include "systemServices/MotionState.hpp"
#include "systemServices/FreeRtosQueue.hpp"
#include "systemServices/GcodeMessage.hpp"


class PlottingManager
{
public:
    PlottingManager(MotionState& ms, FreeRtosQueue<GcodeMessage>& gcodeQueue);

    void init();
    void update();

private:

    void configureDriver(TMC2209Driver& driver);

    // UART
    HardwareSerial driverSerial;

    // Servo
    Servo penServo;

    // Raw drivers
    TMC2209Stepper rawDriverA;
    TMC2209Stepper rawDriverB;

    // Driver abstraction
    TMC2209Driver driverA;
    TMC2209Driver driverB;

    // Motion hardware
    Stepper stepA;
    Stepper stepB;

    StepperAxis axisA;
    StepperAxis axisB;

    ServoPen pen;

    HomingController homingController;

    CoreXYKinematics kinematics;

    MotionSystem motionSystem;

    GCodeParser gcodeParser;

    MotionState& ms;
    FreeRtosQueue<GcodeMessage>& gcodeQueue;
};