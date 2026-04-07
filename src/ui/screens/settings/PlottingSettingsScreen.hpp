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

class PlottingSettingsScreen : public Screen
{
public:
    PlottingSettingsScreen(const ScreensContext& ctx)
    : Screen(
        std::make_unique<widgets::LinearLayout>(
            widgets::LinearLayoutStyle{.axis = widgets::Axis::Vertical},

            std::make_unique<components::HeaderLine>(components::HeaderLineProps{
                .textProvider = "Plotting",
                .wifiStatusProvider = ctx.wifiStatusProvider,
                .onBackPress = [this]() {
                    if (router()) router()->popScreen();
                }
            }),

            std::make_unique<widgets::ScrollableVerticalLayout>(
                widgets::ScrollableVerticalLayoutStyle{},

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "Draw",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .initialValue = static_cast<int>(ctx.runtimeSettings.drawFeedRate_mm_per_s()),
                        .next = [](int current) { return std::min(current + 5, 200); },
                        .prev = [](int current) { return std::max(current - 5, 5); },
                        .onChange = [&sp = ctx.settingsPersistence](const int& newValue) {
                            sp.setDrawFeedRate_mm_per_s(static_cast<float>(newValue));
                        },
                        .toString = [](int value) { return std::to_string(value) + " mm/s"; }
                    }
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "Travel",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .initialValue = static_cast<int>(ctx.runtimeSettings.travelFeedRate_mm_per_s()),
                        .next = [](int current) { return std::min(current + 5, 200); },
                        .prev = [](int current) { return std::max(current - 5, 5); },
                        .onChange = [&sp = ctx.settingsPersistence](const int& newValue) {
                            sp.setTravelFeedRate_mm_per_s(static_cast<float>(newValue));
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