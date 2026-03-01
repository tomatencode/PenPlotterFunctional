#include <Arduino.h>
#include <TMCStepper.h>

#include "config/pins.hpp"
#include "motion/Stepper.hpp"
#include "motion/ServoPen.hpp"
#include "motion/StepperAxis.hpp"
#include "motion/TMC2209Driver.hpp"
#include "motion/CoreXYKinematics.hpp"
#include "motion/MotionSystem.hpp"

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

// abstracted pen
ServoPen pen(penServo, 100, 65); // up position, down position

// Step pulse layer
Stepper stepA(STEP_PIN_A, DIR_PIN_A);
Stepper stepB(STEP_PIN_B, DIR_PIN_B);

// Axis layer
StepperAxis axisA(stepA, driverA, true); // Invert direction for axis A
StepperAxis axisB(stepB, driverB, true); // Invert direction for axis B

// Kinematics
CoreXYKinematics kinematics(5); // 5 steps/mm

// Motion system
MotionSystem motionSystem(axisA, axisB, kinematics);

// sequence variables
float mm_per_s_draw = 20.0;
float mm_per_s_fast = 50.0;

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

    // Set servo
    pinMode(SERVO_PIN, OUTPUT);
    penServo.attach(SERVO_PIN);
    pen.up();
    

    // Enable drivers
    pinMode(EN_PIN, OUTPUT);
    digitalWrite(EN_PIN, LOW);

    driverSerial.begin(115200, SERIAL_8N1, DRIVER_RX_PIN, DRIVER_TX_PIN);

    configureDriver(driverA);
    configureDriver(driverB);
}

// move in a square
void loop() {
    motionSystem.moveToXY({50, 0}, mm_per_s_fast);
    pen.down();
    delay(200);
    motionSystem.arcToXY({- 50, 0}, {0, 0}, true,  mm_per_s_draw);
    motionSystem.arcToXY({ 50, 0}, {0, 0}, true,  mm_per_s_draw);
    delay(200);
    pen.up();
    motionSystem.moveToXY({0, 0}, mm_per_s_fast);

    // stop after drawing
    while (true) {
        delay(1000);
    }
}