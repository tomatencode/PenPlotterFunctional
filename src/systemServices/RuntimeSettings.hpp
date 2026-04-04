#pragma once
#include <atomic>

// ============================================================================
// RuntimeSettings - Lock-free shared state for cross-core setting access
// ============================================================================
//
// Problem:
//   - Core 0 (UI/Web) reads/writes all settings, persists them to NVS
//   - Core 1 (Motion) needs to read feed rates, pen angles, etc. in real-time
//     while executing G-code commands
//   - Can't use Preferences (NVS) from Core 1 — too slow
//   - Can't use mutexes — would block G-code execution
//
// Solution: std::atomic<float> fields
//   - Core 0 updates these atomically when SettingsRepository::update() is called
//   - Core 1 reads them atomically with zero latency, zero locks
//   - Same pattern as MotionState for cross-core communication
//
// Created in: Tasks.cpp (static local)
// Passed to:  ApplicationManager (Core 0) and PlottingManager (Core 1) by reference
//
// Usage from Core 1 (MotionExecuter, GCodeExecuter, ServoPen):
//   float feedRate = _runtimeSettings.drawFeedRate();  // atomic load, lock-free
//
// Usage from Core 0 (SettingsRepository):
//   _runtimeSettings.setDrawFeedRate(20.5f);  // atomic store, lock-free
//

class RuntimeSettings {
public:
    // ====== Setters (called by SettingsRepository on Core 0) ======
    void setDrawFeedRate(float v) { _drawFeedRate.store(v, std::memory_order_relaxed); }
    void setTravelFeedRate(float v) { _travelFeedRate.store(v, std::memory_order_relaxed); }
    void setStepsPerMm(float v) { _stepsPerMm.store(v, std::memory_order_relaxed); }
    void setHomingSpeed(float v) { _homingSpeed.store(v, std::memory_order_relaxed); }
    void setStallguardThreshold(float v) { _stallguardThreshold.store(v, std::memory_order_relaxed); }
    void setPenUpAngle(float v) { _penUpAngle.store(v, std::memory_order_relaxed); }
    void setPenDownAngle(float v) { _penDownAngle.store(v, std::memory_order_relaxed); }

    // ====== Getters (called by Core 1 during motion execution) ======
    float drawFeedRate() const { return _drawFeedRate.load(std::memory_order_relaxed); }
    float travelFeedRate() const { return _travelFeedRate.load(std::memory_order_relaxed); }
    float stepsPerMm() const { return _stepsPerMm.load(std::memory_order_relaxed); }
    float homingSpeed() const { return _homingSpeed.load(std::memory_order_relaxed); }
    float stallguardThreshold() const { return _stallguardThreshold.load(std::memory_order_relaxed); }
    float penUpAngle() const { return _penUpAngle.load(std::memory_order_relaxed); }
    float penDownAngle() const { return _penDownAngle.load(std::memory_order_relaxed); }

private:
    // Using memory_order_relaxed because:
    // - No synchronization required (settings change rarely)
    // - No ordering constraints with other atomics
    // - Fastest possible atomic operations (no barriers)
    
    std::atomic<float> _drawFeedRate{20.0f};          // FEED_RATE_DRAW_MM_PER_S
    std::atomic<float> _travelFeedRate{50.0f};        // FEED_RATE_TRAVEL_MM_PER_S
    std::atomic<float> _stepsPerMm{5.0f};             // STEPS_PER_MM
    std::atomic<float> _homingSpeed{360.0f};          // HOMING_SPEED_STPS_PER_S
    std::atomic<float> _stallguardThreshold{120.0f};  // HOMING_STALLGUARD_THRESHOLD
    std::atomic<float> _penUpAngle{100.0f};           // PEN_UP_ANGLE
    std::atomic<float> _penDownAngle{65.0f};          // PEN_DOWN_ANGLE
};
