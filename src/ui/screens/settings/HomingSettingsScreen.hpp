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
                        .initialValue = static_cast<int>(ctx.runtimeSettings.homingSpeed_stp_per_s()),
                        .next = [](int current) { return std::min(current + 10, 2000); },
                        .prev = [](int current) { return std::max(current - 10, 10); },
                        .onChange = [&sp = ctx.settingsPersistence](const int& newValue) {
                            sp.setHomingSpeed_stp_per_s(static_cast<float>(newValue));
                        },
                        .toString = [](int value) { return std::to_string(value) + " stp/s"; }
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "SG thr",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .initialValue = static_cast<int>(ctx.runtimeSettings.stallguardThreshold()),
                        .next = [](int current) { return std::min(current + 1, 255); },
                        .prev = [](int current) { return std::max(current - 1, 0); },
                        .onChange = [&sp = ctx.settingsPersistence](const int& newValue) {
                            sp.setStallguardThreshold(static_cast<float>(newValue));
                        },
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "SG History",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .initialValue = static_cast<int>(ctx.runtimeSettings.sgHistorySize()),
                        .next = [](int current) { return std::min(current + 1, 20); },
                        .prev = [](int current) { return std::max(current - 1, 1); },
                        .onChange = [&sp = ctx.settingsPersistence](const int& newValue) {
                            sp.setSGHistorySize(static_cast<uint8_t>(newValue));
                        },
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "SG Interval",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .initialValue = static_cast<int>(ctx.runtimeSettings.sgCheckInterval_ms()),
                        .next = [](int current) { return std::min(current + 10, 500); },
                        .prev = [](int current) { return std::max(current - 10, 10); },
                        .onChange = [&sp = ctx.settingsPersistence](const int& newValue) {
                            sp.setSGCheckInterval_ms(static_cast<uint16_t>(newValue));
                        },
                        .toString = [](int value) { return std::to_string(value) + " ms"; }
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "SG Start",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .initialValue = static_cast<int>(ctx.runtimeSettings.sgStartTimeout_ms()),
                        .next = [](int current) { return std::min(current + 10, 1000); },
                        .prev = [](int current) { return std::max(current - 10, 10); },
                        .onChange = [&sp = ctx.settingsPersistence](const int& newValue) {
                            sp.setSGStartTimeout_ms(static_cast<uint16_t>(newValue));
                        },
                        .toString = [](int value) { return std::to_string(value) + " ms"; }
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "BO speed",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .initialValue = static_cast<int>(ctx.runtimeSettings.homingBackOffSpeed_stp_per_s()),
                        .next = [](int current) { return std::min(current + 10, 2000); },
                        .prev = [](int current) { return std::max(current - 10, 10); },
                        .onChange = [&sp = ctx.settingsPersistence](const int& newValue) {
                            sp.setHomingBackOffSpeed_stp_per_s(static_cast<float>(newValue));
                        },
                        .toString = [](int value) { return std::to_string(value) + " stp/s"; }
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "BO steps X",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .initialValue = static_cast<int>(ctx.runtimeSettings.backOffStepsX()),
                        .next = [](int current) { return std::min(current + 1, 100); },
                        .prev = [](int current) { return std::max(current - 1, 0); },
                        .onChange = [&sp = ctx.settingsPersistence](const int& newValue) {
                            sp.setBackOffStepsX(static_cast<uint16_t>(newValue));
                        },
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "BO steps Y",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .initialValue = static_cast<int>(ctx.runtimeSettings.backOffStepsY()),
                        .next = [](int current) { return std::min(current + 1, 100); },
                        .prev = [](int current) { return std::max(current - 1, 0); },
                        .onChange = [&sp = ctx.settingsPersistence](const int& newValue) {
                            sp.setBackOffStepsY(static_cast<uint16_t>(newValue));
                        },
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "Timeout",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .initialValue = static_cast<int>(ctx.runtimeSettings.homingTimeout_us() / 1000000),
                        .next = [](int current) { return std::min(current + 1, 120); },
                        .prev = [](int current) { return std::max(current - 1, 1); },
                        .onChange = [&sp = ctx.settingsPersistence](const int& newValue) {
                            sp.setHomingTimeout_us(static_cast<uint32_t>(newValue) * 1000000UL);
                        },
                        .toString = [](int value) { return std::to_string(value) + " s"; }
                    }
                })
            )
        )
    )
    {}
};

} // namespace screens
} // namespace ui