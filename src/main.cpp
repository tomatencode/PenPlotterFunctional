#include <Arduino.h>
#include <TMCStepper.h>

#include "config/pins.hpp"
#include "motion/Stepper.hpp"
#include "motion/StepperAxis.hpp"
#include "motion/TMC2209Driver.hpp"
#include "motion/CoreXYKinematics.hpp"
#include "motion/CoreXYPlanner.hpp"

// UART
HardwareSerial driverSerial(1);

// Raw TMC drivers
TMC2209Stepper rawDriverA(&driverSerial, R_SENSE, 0);
TMC2209Stepper rawDriverB(&driverSerial, R_SENSE, 2);

// Abstracted drivers
TMC2209Driver driverA(rawDriverA);
TMC2209Driver driverB(rawDriverB);

// Step pulse layer
Stepper stepA(STEP_PIN_A, DIR_PIN_A);
Stepper stepB(STEP_PIN_B, DIR_PIN_B);

// Axis layer
StepperAxis axisA(stepA, driverA);
StepperAxis axisB(stepB, driverB);

// Kinematics
CoreXYKinematics kinematics(5); // 80 steps/mm

void configureDriver(TMC2209Driver& driver) {
    driver.begin();
    driver.setCurrent(1000);
    driver.setMicrosteps(16);
}

void setup() {
    Serial.begin(115200);

    // Mute buzzer
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);

    // Enable drivers
    pinMode(EN_PIN, OUTPUT);
    digitalWrite(EN_PIN, LOW);

    driverSerial.begin(115200, SERIAL_8N1, DRIVER_RX_PIN, DRIVER_TX_PIN);

    configureDriver(driverA);
    configureDriver(driverB);
}

// move in a square

float side_lenght_mm = 100.0f;
float mm_per_s = 50.0f;

void loop() {
    // to top right
    MoveToXY(
        {side_lenght_mm / 2.0f, side_lenght_mm / 2.0f},
        mm_per_s,
        kinematics,
        axisA,
        axisB
    );
    // to top left
    MoveToXY(
        {-side_lenght_mm / 2.0f, side_lenght_mm / 2.0f},
        mm_per_s,
        kinematics,
        axisA,
        axisB
    );
    // to bottom left
    MoveToXY(
        {-side_lenght_mm / 2.0f, -side_lenght_mm / 2.0f},
        mm_per_s,
        kinematics,
        axisA,
        axisB
    );
    // to bottom right
    MoveToXY(
        {side_lenght_mm / 2.0f, -side_lenght_mm / 2.0f},
        mm_per_s,
        kinematics,
        axisA,
        axisB
    );
}