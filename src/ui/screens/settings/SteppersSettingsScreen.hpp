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
                        .getValue = [&rs = ctx.runtimeSettings]() { return static_cast<int>(rs.driverCurrent_mA()); },
                        .next = [&sp = ctx.settingsPersistence](int v) { sp.setDriverCurrent_mA(static_cast<float>(v + 50)); },
                        .prev = [&sp = ctx.settingsPersistence](int v) { sp.setDriverCurrent_mA(static_cast<float>(v - 50)); },
                        .toString = [](const int& value) { return std::to_string(value) + " mA"; }
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "Microsteps",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .getValue = [&rs = ctx.runtimeSettings]() { return static_cast<int>(rs.microsteps()); },
                        .next = [&sp = ctx.settingsPersistence](int v) { sp.setMicrosteps(static_cast<float>(v * 2)); },
                        .prev = [&sp = ctx.settingsPersistence](int v) { sp.setMicrosteps(static_cast<float>(v / 2)); },
                    }
                })
            )
        )
    )
    {}
};

} // namespace screens
} // namespace ui