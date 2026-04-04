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
    float drawFeedRate;
    float travelFeedRate;
    float stepsPerMm;
    float homingSpeed;
    float stallguardThreshold;
};

struct PenSettings {
    float upAngle;
    float downAngle;
};