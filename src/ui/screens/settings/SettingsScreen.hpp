#pragma once

#include <functional>

#include "ui/framework/screen/Screen.hpp"

#include "rtos/MotionState.hpp"
#include "jobController/JobController.hpp"
#include "storage/FileManager.hpp"

#include "settings/SettingPersistence.hpp"
#include "settings/RuntimeSettings.hpp"

// Include related screens to enable navigation
#include "WifiSettingsScreen.hpp"
#include "PlottingSettingsScreen.hpp"
#include "HomingSettingsScreen.hpp"
#include "SteppersSettingsScreen.hpp"
#include "PenSettingsScreen.hpp"
#include "ui/framework/router/Router.hpp"

// Include components and widgets used in this screen
#include "ui/components/HeaderLine.hpp"
#include "ui/components/PressHoldButton.hpp"
#include "ui/framework/widgets/leaves/Button.hpp"
#include "ui/framework/widgets/leaves/Label.hpp"
#include "ui/framework/widgets/layouts/LinearLayout.hpp"

#include "ui/styles/ButtonStyles.hpp"

namespace ui {
namespace screens {

class SettingsScreen : public Screen
{
public:
    SettingsScreen(std::function<bool()> wifiStatusProvider, SettingPersistence& settingPersistence, RuntimeSettings& runtimeSettings)
    : Screen(
        std::make_unique<widgets::LinearLayout>(
            widgets::LinearLayoutStyle{.axis = widgets::Axis::Vertical},

            std::make_unique<components::HeaderLine>(
                "Settings",
                wifiStatusProvider,
                [this]() {
                    if (router()) {
                        router()->popScreen();
                    }
                }
            ),

            std::make_unique<widgets::ScrollableVerticalLayout>(
                widgets::ScrollableVerticalLayoutStyle{},
                
                std::make_unique<widgets::Button>(
                    widgets::ButtonProps{
                        .style = styles::listButtonStyle,
                        .onPress = [this, wifiStatusProvider, &settingPersistence, &runtimeSettings]() {
                            if (router()) {
                                auto wifiSettingsScreen = std::make_unique<WifiSettingsScreen>(wifiStatusProvider, settingPersistence, runtimeSettings);
                                router()->pushScreen(std::move(wifiSettingsScreen));
                            }
                        }
                    },
                    std::make_unique<widgets::Label>("Wifi")
                ),

                std::make_unique<widgets::Button>(
                    widgets::ButtonProps{
                        .style = styles::listButtonStyle,
                        .onPress = [this, wifiStatusProvider, &settingPersistence, &runtimeSettings]() {
                            if (router()) {
                                auto plottingSettingsScreen = std::make_unique<PlottingSettingsScreen>(wifiStatusProvider, settingPersistence, runtimeSettings);
                                router()->pushScreen(std::move(plottingSettingsScreen));
                            }
                        }
                    },
                    std::make_unique<widgets::Label>("Plotting Speeds")
                ),

                std::make_unique<widgets::Button>(
                    widgets::ButtonProps{
                        .style = styles::listButtonStyle,
                        .onPress = [this, wifiStatusProvider, &settingPersistence, &runtimeSettings]() {
                            if (router()) {
                                auto homingSettingsScreen = std::make_unique<HomingSettingsScreen>(wifiStatusProvider, settingPersistence, runtimeSettings);
                                router()->pushScreen(std::move(homingSettingsScreen));
                            }
                        }
                    },
                    std::make_unique<widgets::Label>("Homing Settings")
                ),

                std::make_unique<widgets::Button>(
                    widgets::ButtonProps{
                        .style = styles::listButtonStyle,
                        .onPress = [this, wifiStatusProvider, &settingPersistence, &runtimeSettings]() {
                            if (router()) {
                                auto steppersSettingsScreen = std::make_unique<SteppersSettingsScreen>(wifiStatusProvider, settingPersistence, runtimeSettings);
                                router()->pushScreen(std::move(steppersSettingsScreen));
                            }
                        }
                    },
                    std::make_unique<widgets::Label>("Steppers")
                ),

                std::make_unique<widgets::Button>(
                    widgets::ButtonProps{
                        .style = styles::listButtonStyle,
                        .onPress = [this, wifiStatusProvider, &settingPersistence, &runtimeSettings]() {
                            if (router()) {
                                auto penSettingsScreen = std::make_unique<PenSettingsScreen>(wifiStatusProvider, settingPersistence, runtimeSettings);
                                router()->pushScreen(std::move(penSettingsScreen));
                            }
                        }
                    },
                    std::make_unique<widgets::Label>("Pen")
                ),

                std::make_unique<components::PressHoldButton>(
                    components::PressHoldButtonProps{
                        .onHoldComplete = [&settingPersistence]() {
                            settingPersistence.clearAllSettings();
                        },
                        .holdDurationMs = 3000,
                    }
                )
            )
        )
    )
    {}
};

} // namespace screens
} // namespace ui