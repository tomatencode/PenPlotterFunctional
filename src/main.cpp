#include <Arduino.h>
#include <TMCStepper.h>
#include <vector>

#include "config/pins.hpp"
#include "motion/Stepper.hpp"
#include "motion/ServoPen.hpp"
#include "motion/StepperAxis.hpp"
#include "motion/TMC2209Driver.hpp"
#include "motion/CoreXYKinematics.hpp"
#include "motion/MotionSystem.hpp"
#include "motion/GCodeParser.hpp"
#include "motion/HoamingController.hpp"

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

// Homing controller
HoamingController homingController(axisA, axisB, driverA, driverB, 360, 150.0, 100.0, 1000); // speed_stps_per_s, stallGuard_threshold, sgCheckInterval_ms, acceleration_time_ms

// Kinematics
CoreXYKinematics kinematics(5); // 5 steps/mm

// Motion system
MotionSystem motionSystem(axisA, axisB, kinematics);

// G-code parser
GCodeParser gcodeParser(motionSystem, pen, 20.0, 50.0); // feed rate for drawing, feed rate for travel

// sequence variables
float mm_per_s_draw = 20.0;
float mm_per_s_fast = 50.0;

void configureDriver(TMC2209Driver& driver) {
    driver.begin();
    driver.setStallGuardThreshold(15); // Set stallGuard threshold (adjust as needed)
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

    Serial.println("Setup complete. Ready to receive G-code.");
}

std::vector<String> gcodeLines = {
    // Head (circle with radius 40mm, centered at 50,50)
    "M5",
    "G0 X60 Y100",          // Move to leftmost point of head
    "M3",
    "G2 X60 Y100 I40 J0 F20", // Clockwise full circle (head)
    "M5",

    // Left eye (circle radius 5mm)
    "G0 X80 Y115",          // Move to left eye
    "M3",
    "G2 X80 Y115 I5 J0 F15",   // small circle for left eye
    "M5",

    // Right eye (circle radius 5mm)
    "G0 X110 Y115",          // Move to right eye
    "M3",
    "G2 X110 Y115 I5 J0 F15",   // small circle for right eye
    "M5",

    // Smile (quadratic Bézier)
    "G0 X80 Y85",          // Start of smile
    "M3",
    "G5 X120 Y85 C100 D70 F15", // Control point at (50,20), target at (70,35)
    "M5",

    // Move out of the way
    "G0 X0 Y0 F50"
};

void loop() {
    
    pen.up();
    homingController.home();

    for (const auto& line : gcodeLines) {
        Serial.print("Executing: ");
        Serial.println(line.c_str());
        gcodeParser.executeLine(line.c_str());
        delay(1000); // Short delay between commands for visibility
    }
    
    while (true)
    {
        delay(1000);
    }
    
}