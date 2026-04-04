#pragma once
#include <string>
#include "SettingGroups.hpp"

// ============================================================================
// Observer Interface (same pattern as JobObserver)
// ============================================================================

class SettingsObserver {
public:
    virtual ~SettingsObserver() = default;

    // Each has a default no-op implementation so observers don't need to implement all
    virtual void onNetworkSettingsChanged(const NetworkSettings&) {}
    virtual void onPlottingSettingsChanged(const PlottingSettings&) {}
    virtual void onPenSettingsChanged(const PenSettings&) {}
};
