#pragma once
#include <atomic>

#include "settings/defaults/PlottingDefaults.hpp"

class RuntimeSettings {
public:
    // Setters called by SettingsRepository on Core 0
    void setDriverCurrent_mA(float v) { _driverCurrent_mA.store(v, std::memory_order_relaxed); }
    void setMicrosteps(float v) { _microsteps.store(v, std::memory_order_relaxed); }
    void setDrawFeedRate_mm_per_s(float v) { _drawFeedRate_mm_per_s.store(v, std::memory_order_relaxed); }
    void setTravelFeedRate_mm_per_s(float v) { _travelFeedRate_mm_per_s.store(v, std::memory_order_relaxed); }
    void setMinFeatureSize_mm(float v) { _minFeatureSize_mm.store(v, std::memory_order_relaxed); }
    void setHomingSpeed_stp_per_s(float v) { _homingSpeed_stp_per_s.store(v, std::memory_order_relaxed); }
    void setStallguardThreshold(float v) { _stallguardThreshold.store(v, std::memory_order_relaxed); }
    void setPenUpAngle_deg(float v) { _penUpAngle_deg.store(v, std::memory_order_relaxed); }
    void setPenDownAngle_deg(float v) { _penDownAngle_deg.store(v, std::memory_order_relaxed); }

    // Getters called by Core 1 during motion execution
    float driverCurrent_mA() const { return _driverCurrent_mA.load(std::memory_order_relaxed); }
    float microsteps() const { return _microsteps.load(std::memory_order_relaxed); }
    float drawFeedRate_mm_per_s() const { return _drawFeedRate_mm_per_s.load(std::memory_order_relaxed); }
    float travelFeedRate_mm_per_s() const { return _travelFeedRate_mm_per_s.load(std::memory_order_relaxed); }
    float minFeatureSize_mm() const { return _minFeatureSize_mm.load(std::memory_order_relaxed); }
    float homingSpeed_stp_per_s() const { return _homingSpeed_stp_per_s.load(std::memory_order_relaxed); }
    float stallguardThreshold() const { return _stallguardThreshold.load(std::memory_order_relaxed); }
    float penUpAngle_deg() const { return _penUpAngle_deg.load(std::memory_order_relaxed); }
    float penDownAngle_deg() const { return _penDownAngle_deg.load(std::memory_order_relaxed); }

private:

    std::atomic<float> _driverCurrent_mA{DRIVER_CURRENT_MA};
    std::atomic<float> _microsteps{MICROSTEPS};
    std::atomic<float> _drawFeedRate_mm_per_s{FEED_RATE_DRAW_MM_PER_S};
    std::atomic<float> _travelFeedRate_mm_per_s{FEED_RATE_TRAVEL_MM_PER_S};
    std::atomic<float> _minFeatureSize_mm{MIN_FEATURE_SIZE_MM};
    std::atomic<float> _homingSpeed_stp_per_s{HOMING_SPEED_STP_PER_S};
    std::atomic<float> _stallguardThreshold{HOMING_STALLGUARD_THRESHOLD};
    std::atomic<float> _penUpAngle_deg{PEN_UP_DEG};
    std::atomic<float> _penDownAngle_deg{PEN_DOWN_DEG};
};
