#pragma once

#include <ESP32Servo.h>
#include "Pen.hpp"
#include "settings/RuntimeSettings.hpp"
#include "settings/SettingPersistence.hpp"
#include "settings/SettingObserver.hpp"

// Forward declaration
class RuntimeSettings;

class ServoPen : public Pen, public SettingObserver {
public:
    ServoPen(Servo& servo, SettingPersistence& settingPersistence, RuntimeSettings& runtimeSettings);
    ~ServoPen();

    void onRelevantSettingsChanged() override;

    void down() override;
    void up() override;
    bool isDown() const override;

private:
    Servo& _servo;
    RuntimeSettings& _runtimeSettings;
    SettingPersistence& _settingPersistence;
    bool _penDown;
};