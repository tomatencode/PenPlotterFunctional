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

class PenSettingsScreen : public Screen
{
public:
    PenSettingsScreen(std::function<bool()> wifiStatusProvider, SettingPersistence& settingsPersistence, RuntimeSettings& runtimeSettings)
    : Screen(
        std::make_unique<widgets::LinearLayout>(
            widgets::LinearLayoutStyle{.axis = widgets::Axis::Vertical},

            std::make_unique<components::HeaderLine>(
                "Pen",
                wifiStatusProvider,
                [this]() {
                    if (router()) {
                        router()->popScreen();
                    }
                }
            ),

            std::make_unique<widgets::ScrollableVerticalLayout>(
                widgets::ScrollableVerticalLayoutStyle{},
                
                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "Up Angle",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .initialValue = static_cast<int>(runtimeSettings.penUpAngle_deg()),
                        .next = [&runtimeSettings](int current) { return std::min(current + 1, 180); },
                        .prev = [&runtimeSettings](int current) { return std::max(current - 1, 0); },
                        .onChange = [&settingsPersistence](const int& newValue) {
                            settingsPersistence.setPenUpAngle_deg(static_cast<float>(newValue));
                        },
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "Down Angle",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .initialValue = static_cast<int>(runtimeSettings.penDownAngle_deg()),
                        .next = [&runtimeSettings](int current) { return std::min(current + 1, 180); },
                        .prev = [&runtimeSettings](int current) { return std::max(current - 1, 0); },
                        .onChange = [&settingsPersistence](const int& newValue) {
                            settingsPersistence.setPenDownAngle_deg(static_cast<float>(newValue));
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