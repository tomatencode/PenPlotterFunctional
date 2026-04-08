#pragma once

#include <functional>

#include "rtos/MotionState.hpp"
#include "storage/FileManager.hpp"
#include "gcode/GCodeSender.hpp"
#include "jobController/JobController.hpp"
#include "settings/SettingPersistence.hpp"
#include "settings/RuntimeSettings.hpp"

namespace ui {

struct ScreensContext {
    JobController& jobController;
    GCodeSender& gcodeSender;
    MotionState& motionState;
    FileManager& fileManager;
    SettingPersistence& settingsPersistence;
    RuntimeSettings& runtimeSettings;
    std::function<bool()> wifiStatusProvider;
};

} // namespace ui
