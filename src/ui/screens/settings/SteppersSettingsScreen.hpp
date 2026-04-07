#pragma once

#include "ui/framework/screen/Screen.hpp"
#include "ui/framework/router/Router.hpp"
#include "ui/components/HeaderLine.hpp"
#include "ui/components/LabeledValueSelector.hpp"
#include "ui/framework/widgets/leaves/Button.hpp"
#include "ui/framework/widgets/leaves/Label.hpp"
#include "ui/framework/widgets/layouts/LinearLayout.hpp"
#include "ui/framework/widgets/layouts/ScrollableVerticalLayout.hpp"
#include "ui/screens/ScreensContext.hpp"

namespace ui {
namespace screens {

class SteppersSettingsScreen : public Screen
{
public:
    SteppersSettingsScreen(const ScreensContext& ctx)
    : Screen(
        std::make_unique<widgets::LinearLayout>(
            widgets::LinearLayoutStyle{.axis = widgets::Axis::Vertical},

            std::make_unique<components::HeaderLine>(components::HeaderLineProps{
                .textProvider = "Steppers",
                .wifiStatusProvider = ctx.wifiStatusProvider,
                .onBackPress = [this]() {
                    if (router()) router()->popScreen();
                }
            }),

            std::make_unique<widgets::ScrollableVerticalLayout>(
                widgets::ScrollableVerticalLayoutStyle{},

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "Current",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .initialValue = static_cast<int>(ctx.runtimeSettings.driverCurrent_mA()),
                        .next = [](int current) { return std::min(current + 50, 1500); },
                        .prev = [](int current) { return std::max(current - 50, 100); },
                        .onChange = [&sp = ctx.settingsPersistence](const int& newValue) {
                            sp.setDriverCurrent_mA(static_cast<float>(newValue));
                        },
                        .toString = [](int value) { return std::to_string(value) + " mA"; }
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "Microsteps",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .initialValue = static_cast<int>(ctx.runtimeSettings.microsteps()),
                        .next = [](int current) { return std::min(current * 2, 256); },
                        .prev = [](int current) { return std::max(current / 2, 1); },
                        .onChange = [&sp = ctx.settingsPersistence](const int& newValue) {
                            sp.setMicrosteps(static_cast<float>(newValue));
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