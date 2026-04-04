#pragma once
#include <string>

// ============================================================================
// Settings Data Structures (plain POD for easy copying and serialization)
// ============================================================================

struct NetworkSettings {
    std::string ssid;
    std::string password;
    std::string mdnsName;
};

struct PlottingSettings {
    float driverCurrent_mA;
    float microsteps;
    float drawFeedRate_mm_per_s;
    float travelFeedRate_mm_per_s;
    float homingSpeed_stp_per_s;
    float stallguardThreshold;
};

struct PenSettings {
    float upAngle_deg;
    float downAngle_deg;
};