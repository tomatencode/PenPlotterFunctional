#pragma once
#include "SettingsObserver.hpp"
#include "RuntimeSettings.hpp"
#include <vector>
#include <memory>

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
