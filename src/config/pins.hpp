#pragma once
#include <cstdint>

// Workspace
constexpr float MAX_X_MM = 185.0f;
constexpr float MAX_Y_MM = 265.0f;

// Motor pins
constexpr uint8_t STEP_PIN_A = 19;
constexpr uint8_t DIR_PIN_A  = 4;
constexpr uint8_t STEP_PIN_B = 18;
constexpr uint8_t DIR_PIN_B  = 5;
constexpr uint8_t EN_PIN     = 23;

// Driver UART
constexpr uint8_t DRIVER_TX_PIN = 17;
constexpr uint8_t DRIVER_RX_PIN = 16;
constexpr float R_SENSE = 0.11f;

// buzzer
constexpr uint8_t BUZZER_PIN = 14;

// servo
constexpr uint8_t SERVO_PIN = 13;