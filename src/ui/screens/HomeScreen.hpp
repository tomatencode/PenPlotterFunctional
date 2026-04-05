#pragma once

#include <functional>

#include "../framework/screen/Screen.hpp"

#include "rtos/MotionState.hpp"
#include "jobController/JobController.hpp"
#include "storage/FileManager.hpp"

// Include related screens to enable navigation
#include "plotting/FilesScreen.hpp"
#include "settings/SettingsScreen.hpp"
#include "../framework/router/Router.hpp"

// Include components and widgets used in this screen
#include "../components/HeaderLine.hpp"
#include "../framework/widgets/leaves/Button.hpp"
#include "../framework/widgets/leaves/Label.hpp"
#include "../framework/widgets/layouts/LinearLayout.hpp"

namespace ui {
namespace screens {

// The main screen of the UI
class HomeScreen : public Screen
{
public:
    HomeScreen(JobController& jobController,
               MotionState& motionState,
               FileManager& fileManager,
               std::function<bool()> wifiStatusProvider,
               SettingPersistence& settingsPersistence,
               RuntimeSettings& runtimeSettings
              )
    : Screen(
        std::make_unique<widgets::LinearLayout>(
            widgets::LinearLayoutStyle{.axis = widgets::Axis::Vertical, .horizontalAlign = widgets::HorizontalAlignment::Center},
            std::make_unique<components::HeaderLine>(components::HeaderLineProps{
                .textProvider = "Pen Plotter",
                .wifiStatusProvider = wifiStatusProvider
            }),

            std::make_unique<widgets::Button>(
                widgets::ButtonProps{
                    .onPress = [this, &jobController, &motionState, &fileManager, wifiStatusProvider]() {
                        if (router()) {
                            auto filesScreen = std::make_unique<FilesScreen>(jobController, motionState, fileManager, wifiStatusProvider);
                            router()->pushScreen(std::move(filesScreen));
                        }
                    }
                },
                std::make_unique<widgets::Label>("Plot")
            ),

            std::make_unique<widgets::Button>(
                widgets::ButtonProps{
                    .onPress = [this, wifiStatusProvider, &settingsPersistence, &runtimeSettings]() {
                        if (router()) {
                            auto settingsScreen = std::make_unique<SettingsScreen>(wifiStatusProvider, settingsPersistence, runtimeSettings);
                            router()->pushScreen(std::move(settingsScreen));
                        }
                    }
                },
                std::make_unique<widgets::Label>("Settings")
            )
        )
    , 0) // Start with Plot button focused
    {}
};

} // namespace screens
} // namespace ui