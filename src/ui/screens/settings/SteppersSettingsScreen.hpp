#pragma once

#include <functional>

#include "ui/framework/screen/Screen.hpp"

#include "rtos/MotionState.hpp"
#include "jobController/JobController.hpp"
#include "storage/FileManager.hpp"

#include "settings/SettingPersistence.hpp"
#include "settings/RuntimeSettings.hpp"

// Include related screens to enable navigation
#include "ui/framework/router/Router.hpp"

// Include components and widgets used in this screen
#include "ui/components/HeaderLine.hpp"
#include "ui/components/LabeledValueSelector.hpp"
#include "ui/framework/widgets/leaves/Button.hpp"
#include "ui/framework/widgets/leaves/Label.hpp"
#include "ui/framework/widgets/layouts/LinearLayout.hpp"
#include "ui/framework/widgets/layouts/ScrollableVerticalLayout.hpp"

namespace ui {
namespace screens {

class SteppersSettingsScreen : public Screen
{
public:
    SteppersSettingsScreen(std::function<bool()> wifiStatusProvider, SettingPersistence& settingsPersistence, RuntimeSettings& runtimeSettings)
    : Screen(
        std::make_unique<widgets::LinearLayout>(
            widgets::LinearLayoutStyle{.axis = widgets::Axis::Vertical},

            std::make_unique<components::HeaderLine>(components::HeaderLineProps{
                .textProvider = "Steppers",
                .wifiStatusProvider = wifiStatusProvider,
                .onBackPress = [this]() {
                    if (router()) router()->popScreen();
                }
            }),

            std::make_unique<widgets::ScrollableVerticalLayout>(
                widgets::ScrollableVerticalLayoutStyle{},

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "Current",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .initialValue = static_cast<int>(runtimeSettings.driverCurrent_mA()),
                        .next = [](int current) { return std::min(current + 50, 1500); },
                        .prev = [](int current) { return std::max(current - 50, 100); },
                        .onChange = [&settingsPersistence](const int& newValue) {
                            settingsPersistence.setDriverCurrent_mA(static_cast<float>(newValue));
                        },
                        .toString = [](int value) { return std::to_string(value) + " mA"; }
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "Microsteps",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .initialValue = static_cast<int>(runtimeSettings.microsteps()),
                        .next = [](int current) { return std::min(current * 2, 256); },
                        .prev = [](int current) { return std::max(current / 2, 1); },
                        .onChange = [&settingsPersistence](const int& newValue) {
                            settingsPersistence.setMicrosteps(static_cast<float>(newValue));
                        },
                    }
                })
            )
        )
    )
    {}
};

} // namespace screens
} // namespace ui