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

class PlottingSettingsScreen : public Screen
{
public:
    PlottingSettingsScreen(std::function<bool()> wifiStatusProvider, SettingPersistence& settingsPersistence, RuntimeSettings& runtimeSettings)
    : Screen(
        std::make_unique<widgets::LinearLayout>(
            widgets::LinearLayoutStyle{.axis = widgets::Axis::Vertical},

            std::make_unique<components::HeaderLine>(
                "Plotting",
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
                    .labelText = "Draw",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .initialValue = static_cast<int>(runtimeSettings.drawFeedRate_mm_per_s()),
                        .next = [](int current) { return std::min(current + 1, 200); },
                        .prev = [](int current) { return std::max(current - 1, 1); },
                        .onChange = [&settingsPersistence](const int& newValue) {
                            settingsPersistence.setDrawFeedRate_mm_per_s(static_cast<float>(newValue));
                        },
                        .toString = [](int value) { return std::to_string(value) + " mm/s"; }
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "Travel",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .initialValue = static_cast<int>(runtimeSettings.travelFeedRate_mm_per_s()),
                        .next = [](int current) { return std::min(current + 1, 200); },
                        .prev = [](int current) { return std::max(current - 1, 1); },
                        .onChange = [&settingsPersistence](const int& newValue) {
                            settingsPersistence.setTravelFeedRate_mm_per_s(static_cast<float>(newValue));
                        },
                        .toString = [](int value) { return std::to_string(value) + " mm/s"; }
                    }
                })
            )
        )
    )
    {}
};

} // namespace screens
} // namespace ui