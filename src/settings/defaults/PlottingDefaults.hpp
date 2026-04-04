#pragma once
#include <cstdint>

// Pen
constexpr float PEN_UP_DEG = 100.0f;
constexpr float PEN_DOWN_DEG = 65.0f;

// Driver
constexpr float DRIVER_CURRENT_MA = 1000.0f;
constexpr uint16_t MICROSTEPS = 16;

// Homing
constexpr float HOMING_SPEED_STP_PER_S = 360.0f;
constexpr float HOMING_STALLGUARD_THRESHOLD = 120.0f;
constexpr float HOMING_SG_CHECK_INTERVAL_MS = 50.0f;
constexpr uint16_t HOMING_CONSECUTIVE_STALL_CHECKS = 5;
constexpr uint16_t HOMING_SG_START_TIMEOUT_MS = 500;

// Movement Speeds
constexpr float FEED_RATE_DRAW_MM_PER_S = 20.0f;
constexpr float FEED_RATE_TRAVEL_MM_PER_S = 50.0f;

// Minimum feature size
constexpr float MIN_FEATURE_SIZE_MM = 1.0f;