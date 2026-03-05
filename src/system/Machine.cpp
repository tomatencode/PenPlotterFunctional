#include "Machine.hpp"

#include <Arduino.h>
#include <TMCStepper.h>
#include <vector>

#include "../config/pins.hpp"
#include "../motion/Stepper.hpp"
#include "../motion/ServoPen.hpp"
#include "../motion/StepperAxis.hpp"
#include "../motion/TMC2209Driver.hpp"
#include "../motion/CoreXYKinematics.hpp"
#include "../motion/MotionSystem.hpp"
#include "../motion/HomingController.hpp"

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
ServoPen pen(penServo, 100, 65);

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
    360,
    200.0,
    100.0,
    1000);

// Kinematics
CoreXYKinematics kinematics(5);

// Motion system
MotionSystem motionSystem(axisA, axisB, kinematics);

// Global G-code parser
GCodeParser gcodeParser(motionSystem, pen, homingController, 20.0, 50.0);

static void configureDriver(TMC2209Driver& driver)
{
    driver.begin();
    driver.setStallGuardThreshold(15);
    driver.setCurrent(1000);
    driver.setMicrosteps(16);
}

void initMachine()
{
    Serial.begin(115200);

    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);

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