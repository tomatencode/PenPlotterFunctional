#pragma once
#include <ESP32Servo.h>
#include "Pen.hpp"
#include "rtos/MotionState.hpp"
#include "settings/RuntimeSettings.hpp"
#include "settings/SettingPersistence.hpp"
#include "settings/SettingObserver.hpp"

#include <cstdint>

class Pen : public SettingObserver {
public:
    Pen(Servo& servo, MotionState& motionState, SettingPersistence& settingPersistence, RuntimeSettings& runtimeSettings);
    ~Pen();

    void init();

    void down();
    void up();

    void onRelevantSettingsChanged() override;

private:
    Servo& _servo;
    MotionState& _motionState;
    RuntimeSettings& _runtimeSettings;
    SettingPersistence& _settingPersistence;
};