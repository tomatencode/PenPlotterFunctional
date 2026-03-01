#include <Arduino.h>
#include <TMCStepper.h>
#include <Servo.h>

#include "config/pins.hpp"
#include "motion/Stepper.hpp"
#include "motion/StepperAxis.hpp"
#include "motion/TMC2209Driver.hpp"
#include "motion/CoreXYKinematics.hpp"
#include "motion/MotionSystem.hpp"
#include "examples/drawText.hpp"

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
float side_lenght_mm = 100.0f;
float mm_per_s_draw = 20.0f;
float mm_per_s_fast = 50.0f;

float servo_up_pos = 100.0f;
float servo_down_pos = 65.0f;

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
    penServo.write(servo_up_pos);
    

    // Enable drivers
    pinMode(EN_PIN, OUTPUT);
    digitalWrite(EN_PIN, LOW);

    driverSerial.begin(115200, SERIAL_8N1, DRIVER_RX_PIN, DRIVER_TX_PIN);

    configureDriver(driverA);
    configureDriver(driverB);
}

// move in a square
void loop() {
    
    // Example: Draw "HELLO WORLD!" at position (0, 0) with size 12mm
    DrawText(
        "HELLO WORLD!",    // Text to draw
        0.0f,              // X position (mm)
        0.0f,              // Y position (mm)
        12.0f,             // Font size (mm)
        mm_per_s_draw,     // Drawing speed (mm/s)
        mm_per_s_fast,     // Moving speed (mm/s)
        motionSystem,      // Motion system
        penServo,          // Servo
        servo_up_pos,      // Pen up angle
        servo_down_pos     // Pen down angle
    );

    // Example: Draw all uppercase letters at position (-20, 0) with size 5mm
    DrawText(
        "ABCDEFGHIJKLMNOPQRST",    // Text to draw
        0.0f,              // X position (mm)
        -20.0f,              // Y position (mm)
        8.0f,              // Font size (mm)
        mm_per_s_draw,     // Drawing speed (mm/s)
        mm_per_s_fast,     // Moving speed (mm/s)
        motionSystem,      // Motion system
        penServo,          // Servo
        servo_up_pos,      // Pen up angle
        servo_down_pos     // Pen down angle
    );
    DrawText(
        "UVWXYZ1234567890!?.,",    // Text to draw
        0.0f,              // X position (mm)
        -32.0f,             // Y position (mm)
        8.0f,              // Font size (mm)
        mm_per_s_draw,     // Drawing speed (mm/s)
        mm_per_s_fast,     // Moving speed (mm/s)
        motionSystem,      // Motion system
        penServo,          // Servo
        servo_up_pos,      // Pen up angle
        servo_down_pos     // Pen down angle
    );
    // stop after drawing
    while (true) {
        delay(1000);
    }
}