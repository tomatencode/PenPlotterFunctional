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

class HomingSettingsScreen : public Screen
{
public:
    HomingSettingsScreen(std::function<bool()> wifiStatusProvider, SettingPersistence& settingsPersistence, RuntimeSettings& runtimeSettings)
    : Screen(
        std::make_unique<widgets::LinearLayout>(
            widgets::LinearLayoutStyle{.axis = widgets::Axis::Vertical},

            std::make_unique<components::HeaderLine>(components::HeaderLineProps{
                .textProvider = "Homing",
                .wifiStatusProvider = wifiStatusProvider,
                .onBackPress = [this]() {
                    if (router()) router()->popScreen();
                }
            }),

            std::make_unique<widgets::ScrollableVerticalLayout>(
                widgets::ScrollableVerticalLayoutStyle{},

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "Speed",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .initialValue = static_cast<int>(runtimeSettings.homingSpeed_stp_per_s()),
                        .next = [](int current) { return std::min(current + 10, 2000); },
                        .prev = [](int current) { return std::max(current - 10, 10); },
                        .onChange = [&settingsPersistence](const int& newValue) {
                            settingsPersistence.setHomingSpeed_stp_per_s(static_cast<float>(newValue));
                        },
                        .toString = [](int value) { return std::to_string(value) + " stp/s"; }
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "Stallguard",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .initialValue = static_cast<int>(runtimeSettings.stallguardThreshold()),
                        .next = [](int current) { return std::min(current + 1, 255); },
                        .prev = [](int current) { return std::max(current - 1, 0); },
                        .onChange = [&settingsPersistence](const int& newValue) {
                            settingsPersistence.setStallguardThreshold(static_cast<float>(newValue));
                        },
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "BO speed",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .initialValue = static_cast<int>(runtimeSettings.homingBackOffSpeed_stp_per_s()),
                        .next = [](int current) { return std::min(current + 10, 2000); },
                        .prev = [](int current) { return std::max(current - 10, 10); },
                        .onChange = [&settingsPersistence](const int& newValue) {
                            settingsPersistence.setHomingBackOffSpeed_stp_per_s(static_cast<float>(newValue));
                        },
                        .toString = [](int value) { return std::to_string(value) + " stp/s"; }
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "BO steps X",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .initialValue = static_cast<int>(runtimeSettings.backOffStepsX()),
                        .next = [](int current) { return std::min(current + 1, 100); },
                        .prev = [](int current) { return std::max(current - 1, 0); },
                        .onChange = [&settingsPersistence](const int& newValue) {
                            settingsPersistence.setBackOffStepsX(static_cast<uint16_t>(newValue));
                        },
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "BO steps Y",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .initialValue = static_cast<int>(runtimeSettings.backOffStepsY()),
                        .next = [](int current) { return std::min(current + 1, 100); },
                        .prev = [](int current) { return std::max(current - 1, 0); },
                        .onChange = [&settingsPersistence](const int& newValue) {
                            settingsPersistence.setBackOffStepsY(static_cast<uint16_t>(newValue));
                        },
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "Timeout (s)",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .initialValue = static_cast<int>(runtimeSettings.homingTimeout_us() / 1000000),
                        .next = [](int current) { return std::min(current + 1, 120); },
                        .prev = [](int current) { return std::max(current - 1, 1); },
                        .onChange = [&settingsPersistence](const int& newValue) {
                            settingsPersistence.setHomingTimeout_us(static_cast<uint32_t>(newValue) * 1000000UL);
                        },
                        .toString = [](int value) { return std::to_string(value) + " s"; }
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "SG Interval (ms)",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .initialValue = static_cast<int>(runtimeSettings.sgCheckInterval_ms()),
                        .next = [](int current) { return std::min(current + 10, 500); },
                        .prev = [](int current) { return std::max(current - 10, 10); },
                        .onChange = [&settingsPersistence](const int& newValue) {
                            settingsPersistence.setSGCheckInterval_ms(static_cast<uint16_t>(newValue));
                        },
                        .toString = [](int value) { return std::to_string(value) + " ms"; }
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "SG Start (ms)",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .initialValue = static_cast<int>(runtimeSettings.sgStartTimeout_ms()),
                        .next = [](int current) { return std::min(current + 10, 1000); },
                        .prev = [](int current) { return std::max(current - 10, 10); },
                        .onChange = [&settingsPersistence](const int& newValue) {
                            settingsPersistence.setSGStartTimeout_ms(static_cast<uint16_t>(newValue));
                        },
                        .toString = [](int value) { return std::to_string(value) + " ms"; }
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "SG History",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .initialValue = static_cast<int>(runtimeSettings.sgHistorySize()),
                        .next = [](int current) { return std::min(current + 1, 20); },
                        .prev = [](int current) { return std::max(current - 1, 1); },
                        .onChange = [&settingsPersistence](const int& newValue) {
                            settingsPersistence.setSGHistorySize(static_cast<uint8_t>(newValue));
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