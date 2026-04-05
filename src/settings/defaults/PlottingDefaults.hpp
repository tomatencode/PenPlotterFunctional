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
constexpr float STALLGUARD_THRESHOLD = 120.0f;

// Movement Speeds
constexpr float FEED_RATE_DRAW_MM_PER_S = 20.0f;
constexpr float FEED_RATE_TRAVEL_MM_PER_S = 50.0f;

// Minimum feature size
constexpr float MIN_FEATURE_SIZE_MM = 1.0f;