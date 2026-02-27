#include <Arduino.h>
#include <TMCStepper.h>
#include "motion/Stepper.hpp"
#include "motion/CoreXYPlanner.hpp"
#include "motion/CoreXYKinematics.hpp"
#include "motion/StepperAxis.hpp"

// === Workspace dimensions ===
#define maxX_mm 185.0f
#define maxY_mm 265.0f

// === Pins and driver ===
#define STEP_PIN_A 19
#define DIR_PIN_A 4
#define STEP_PIN_B 18
#define DIR_PIN_B 5
#define EN_PIN 23

#define DRIVER_TX_PIN 17
#define DRIVER_RX_PIN 16
#define R_SENSE 0.11f

HardwareSerial driverSerial(1);

TMC2209Stepper driverA(&driverSerial, R_SENSE, 0);
TMC2209Stepper driverB(&driverSerial, R_SENSE, 2);
Stepper stepA(STEP_PIN_A, DIR_PIN_A);
Stepper stepB(STEP_PIN_B, DIR_PIN_B);
StepperAxis axisA(stepA, [](){ return 16.0f; });
StepperAxis axisB(stepB, [](){ return 16.0f; });


void setup() {
  // == MUTE BUZZER ==
  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);

  // === ENABLE DRIVERS ===
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);

  driverSerial.begin(115200, SERIAL_8N1, DRIVER_RX_PIN, DRIVER_TX_PIN);
  // === INIT DRIVER A ===
  driverA.begin();
  driverA.toff(5);
  driverA.rms_current(1000);
  driverA.microsteps(16);
  driverA.en_spreadCycle(false);
  driverA.pwm_autoscale(true);

  // === INIT DRIVER B ===
  driverB.begin();
  driverB.toff(5);
  driverB.rms_current(1000);
  driverB.microsteps(16);
  driverB.en_spreadCycle(false);
  driverB.pwm_autoscale(true);
}

void loop() {
  // put your main code here, to run repeatedly:
}