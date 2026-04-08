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

class HomingSettingsScreen : public Screen
{
public:
    HomingSettingsScreen(const ScreensContext& ctx)
    : Screen(
        std::make_unique<widgets::LinearLayout>(
            widgets::LinearLayoutStyle{.axis = widgets::Axis::Vertical},

            std::make_unique<components::HeaderLine>(components::HeaderLineProps{
                .textProvider = "Homing",
                .wifiStatusProvider = ctx.wifiStatusProvider,
                .onBackPress = [this]() {
                    if (router()) router()->popScreen();
                }
            }),

            std::make_unique<widgets::ScrollableVerticalLayout>(
                widgets::ScrollableVerticalLayoutStyle{},

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "Speed",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .getValue = [&rs = ctx.runtimeSettings]() { return static_cast<int>(rs.homingSpeed_stp_per_s()); },
                        .next = [&sp = ctx.settingsPersistence](int v) { sp.setHomingSpeed_stp_per_s(static_cast<float>(v + 10)); },
                        .prev = [&sp = ctx.settingsPersistence](int v) { sp.setHomingSpeed_stp_per_s(static_cast<float>(v - 10)); },
                        .toString = [](const int& value) { return std::to_string(value) + " stp/s"; }
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "SG thr",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .getValue = [&rs = ctx.runtimeSettings]() { return static_cast<int>(rs.stallguardThreshold()); },
                        .next = [&sp = ctx.settingsPersistence](int v) { sp.setStallguardThreshold(static_cast<float>(v + 1)); },
                        .prev = [&sp = ctx.settingsPersistence](int v) { sp.setStallguardThreshold(static_cast<float>(v - 1)); },
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "SG History",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .getValue = [&rs = ctx.runtimeSettings]() { return static_cast<int>(rs.sgHistorySize()); },
                        .next = [&sp = ctx.settingsPersistence](int v) { sp.setSGHistorySize(static_cast<uint8_t>(v + 1)); },
                        .prev = [&sp = ctx.settingsPersistence](int v) { sp.setSGHistorySize(static_cast<uint8_t>(v - 1)); },
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "SG Interval",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .getValue = [&rs = ctx.runtimeSettings]() { return static_cast<int>(rs.sgCheckInterval_ms()); },
                        .next = [&sp = ctx.settingsPersistence](int v) { sp.setSGCheckInterval_ms(static_cast<uint16_t>(v + 10)); },
                        .prev = [&sp = ctx.settingsPersistence](int v) { sp.setSGCheckInterval_ms(static_cast<uint16_t>(v - 10)); },
                        .toString = [](const int& value) { return std::to_string(value) + " ms"; }
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "SG Start",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .getValue = [&rs = ctx.runtimeSettings]() { return static_cast<int>(rs.sgStartTimeout_ms()); },
                        .next = [&sp = ctx.settingsPersistence](int v) { sp.setSGStartTimeout_ms(static_cast<uint16_t>(v + 10)); },
                        .prev = [&sp = ctx.settingsPersistence](int v) { sp.setSGStartTimeout_ms(static_cast<uint16_t>(v - 10)); },
                        .toString = [](const int& value) { return std::to_string(value) + " ms"; }
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "BO speed",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .getValue = [&rs = ctx.runtimeSettings]() { return static_cast<int>(rs.homingBackOffSpeed_stp_per_s()); },
                        .next = [&sp = ctx.settingsPersistence](int v) { sp.setHomingBackOffSpeed_stp_per_s(static_cast<float>(v + 10)); },
                        .prev = [&sp = ctx.settingsPersistence](int v) { sp.setHomingBackOffSpeed_stp_per_s(static_cast<float>(v - 10)); },
                        .toString = [](const int& value) { return std::to_string(value) + " stp/s"; }
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "BO steps X",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .getValue = [&rs = ctx.runtimeSettings]() { return static_cast<int>(rs.backOffStepsX()); },
                        .next = [&sp = ctx.settingsPersistence](int v) { sp.setBackOffStepsX(static_cast<uint16_t>(v + 1)); },
                        .prev = [&sp = ctx.settingsPersistence](int v) { sp.setBackOffStepsX(static_cast<uint16_t>(v - 1)); },
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "BO steps Y",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .getValue = [&rs = ctx.runtimeSettings]() { return static_cast<int>(rs.backOffStepsY()); },
                        .next = [&sp = ctx.settingsPersistence](int v) { sp.setBackOffStepsY(static_cast<uint16_t>(v + 1)); },
                        .prev = [&sp = ctx.settingsPersistence](int v) { sp.setBackOffStepsY(static_cast<uint16_t>(v - 1)); },
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "Timeout",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .getValue = [&rs = ctx.runtimeSettings]() { return static_cast<int>(rs.homingTimeout_us() / 1000000); },
                        .next = [&sp = ctx.settingsPersistence](int v) { sp.setHomingTimeout_us(static_cast<uint32_t>(v + 1) * 1000000UL); },
                        .prev = [&sp = ctx.settingsPersistence](int v) { sp.setHomingTimeout_us(static_cast<uint32_t>(v) * 1000000UL); },
                        .toString = [](const int& value) { return std::to_string(value) + " s"; }
                    }
                })
            )
        )
    )
    {}
};

} // namespace screens
} // namespace ui