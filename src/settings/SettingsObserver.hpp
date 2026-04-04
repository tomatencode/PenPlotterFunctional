#pragma once
#include "Settings.hpp"
#include <set>
#include <initializer_list>
#include <atomic>

class SettingsObserver {
public:
    virtual ~SettingsObserver() = default;

    // Constructor: declare which settings this observer is interested in
    // Example: SettingsObserver({Setting::DrawFeedRate, Setting::PenUpAngle})
    explicit SettingsObserver(std::initializer_list<Setting> interestedSettings)
        : _interestedSettings(interestedSettings.begin(), interestedSettings.end()) {}

    // Check if this observer cares about a particular setting
    bool isInterestedIn(Setting setting) const {
        return _interestedSettings.count(setting) > 0;
    }

    void markChanged() {
        _didChange.store(true);
    }

protected:

    void checkIfChanged() {
        if (_didChange.exchange(false)) {  // Atomic read-and-clear
            onRelevantSettingsChanged();
        }
    }

    virtual void onRelevantSettingsChanged() = 0;

private:
    std::atomic<bool> _didChange = false;
    std::set<Setting> _interestedSettings;
};
