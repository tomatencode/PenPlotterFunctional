#include "PlottingManager.hpp"

#include <Arduino.h>
#include <TMCStepper.h>
#include <vector>

#include "config/pins.hpp"
#include "config/machine_config.hpp"
#include "hardware/steppers/Stepper.hpp"
#include "hardware/pen/ServoPen.hpp"
#include "hardware/axis/StepperAxis.hpp"
#include "hardware/drivers/TMC2209Driver.hpp"
#include "motion/CoreXYKinematics.hpp"
#include "motion/MotionSystem.hpp"
#include "motion/HomingController.hpp"

#include "systemServices/Queues.hpp"
#include "systemServices/shared/SharedData.hpp"

// UART
HardwareSerial driverSerial(1);

// servo
Servo penServo;

// Raw TMC drivers
TMC2209Stepper rawDriverA(&driverSerial, R_SENSE, 0);
TMC2209Stepper rawDriverB(&driverSerial, R_SENSE, 2);

// Abstracted drivers
TMC2209Driver driverA(rawDriverA);
TMC2209Driver driverB(rawDriverB);

// Pen abstraction
ServoPen pen(penServo, PEN_UP_ANGLE, PEN_DOWN_ANGLE);

// Step pulse layer
Stepper stepA(STEP_PIN_A, DIR_PIN_A);
Stepper stepB(STEP_PIN_B, DIR_PIN_B);

// Axis layer
StepperAxis axisA(stepA, driverA, true);
StepperAxis axisB(stepB, driverB, true);

// Homing
HomingController homingController(
    axisA,
    axisB,
    driverA,
    driverB,
    HOMING_SPEED_STPS_PER_S,
    HOMING_STALLGUARD_THRESHOLD,
    HOMING_SG_CHECK_INTERVAL_MS,
    HOMING_CONSECUTIVE_STALL_CHECKS,
    HOMING_SG_START_TIMEOUT_MS);

// Kinematics
CoreXYKinematics kinematics(STEPS_PER_MM);

// Motion system
MotionSystem motionSystem(axisA, axisB, kinematics);

// Global G-code parser
GCodeParser gcodeParser(motionSystem, pen, homingController, FEED_RATE_DRAW_MM_PER_S, FEED_RATE_TRAVEL_MM_PER_S);

static void configureDriver(TMC2209Driver& driver)
{
    driver.begin();
    driver.setStallGuardThreshold(HOMING_STALLGUARD_THRESHOLD);
    driver.setCurrent(CURRENT_MA);
    driver.setMicrosteps(MICROSTEPS);
}

void plottingManagerInit()
{
    pinMode(SERVO_PIN, OUTPUT);
    penServo.attach(SERVO_PIN);
    pen.up();

    pinMode(EN_PIN, OUTPUT);
    digitalWrite(EN_PIN, LOW);

    driverSerial.begin(115200, SERIAL_8N1, DRIVER_RX_PIN, DRIVER_TX_PIN);

    configureDriver(driverA);
    configureDriver(driverB);

    Serial.println("Machine initialized.");
}

void plottingManagerUpdate()
{
    GcodeMessage msg;

    if (xQueueReceive(gcodeQueue, &msg, 0) == pdTRUE && motionCommand != MotionCommand::ABORT)
    {
        telemetry.currentLineNumber = msg.lineNumber;
        telemetry.state = MotionState::RUNNING;

        Serial.print("Executing line ");
        Serial.print(msg.lineNumber);
        Serial.print(": ");
        Serial.println(msg.line);

        gcodeParser.executeLine(msg.line);
    }
    else
    {
        // Queue empty or aborting
        telemetry.state = MotionState::IDLE;
    }
}