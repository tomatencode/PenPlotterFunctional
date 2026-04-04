#pragma once
#include "SettingsObserver.hpp"
#include <vector>
#include <memory>

// Forward declarations
class RuntimeSettings;

// ============================================================================
// SettingsRepository - Single source of truth for all runtime settings
// ============================================================================
//
// Responsibilities:
// 1. Load/persist settings from/to ESP32 NVS (Non-Volatile Storage / flash)
// 2. Provide thread-safe getters for current settings (return by value)
// 3. Handle updates to settings (validate, persist, notify observers, update RuntimeSettings)
// 4. Manage observer notifications when settings change
//
// Design:
// - Grouped by domain (Network, Machine, Pen) → one struct per group
// - Each group has: getter(), update(), NVS namespace
// - All NVS logic is hidden inside update() methods
// - No confusion about when things persist or take effect
// - Observers get notified with the full struct (batch notification, not per-field)
//
// Usage:
//   repo.updatePlottingSettings(PlottingSettings{20.0f, 50.0f, 5.0f, ...});
//   // ^ Automatically:
//   //   - validates
//   //   - writes all fields to NVS namespace "machine"
//   //   - updates RuntimeSettings (atomic bridge to Core 1)
//   //   - notifies all observers of the change
//

class SettingsRepository {
public:
    explicit SettingsRepository(RuntimeSettings& runtimeSettings);

    // Initialize: load all settings from NVS at startup
    // If a setting key doesn't exist in NVS, uses the default from config headers
    void init();

    // ====== Getters: return current values (fast, no I/O) ======
    NetworkSettings getNetworkSettings() const;
    PlottingSettings getPlottingSettings() const;
    PenSettings getPenSettings() const;

    // ====== Setters: update value, persist to NVS, update RuntimeSettings, notify ======
    // Each takes a struct by const reference, validates it if needed, then:
    // 1. Persists all fields to the corresponding NVS namespace
    // 2. Updates the in-RAM copy
    // 3. Updates RuntimeSettings (the atomic bridge to Core 1)
    // 4. Notifies all observers
    void updateNetworkSettings(const NetworkSettings& settings);
    void updatePlottingSettings(const PlottingSettings& settings);
    void updatePenSettings(const PenSettings& settings);

    // ====== Observer Management ======
    void addObserver(SettingsObserver* observer);
    void removeObserver(SettingsObserver* observer);

private:
    // Notify all observers of a group change
    void notifyNetworkSettingsChanged() const;
    void notifyPlottingSettingsChanged() const;
    void notifyPenSettingsChanged() const;

    // Persist a settings group to NVS
    void persistNetworkSettings();
    void persistPlottingSettings();
    void persistPenSettings();

    // Load a settings group from NVS (with hardcoded defaults as fallback)
    void loadNetworkSettings();
    void loadPlottingSettings();
    void loadPenSettings();

    // ====== Data ======
    NetworkSettings _network;
    PlottingSettings _plotting;
    PenSettings _pen;

    std::vector<SettingsObserver*> _observers;

    RuntimeSettings& _runtimeSettings;  // Reference to cross-core atomic bridge
};
