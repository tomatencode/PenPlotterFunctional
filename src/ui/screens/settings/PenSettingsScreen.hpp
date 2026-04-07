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

class PenSettingsScreen : public Screen
{
public:
    PenSettingsScreen(const ScreensContext& ctx)
    : Screen(
        std::make_unique<widgets::LinearLayout>(
            widgets::LinearLayoutStyle{.axis = widgets::Axis::Vertical},

            std::make_unique<components::HeaderLine>(components::HeaderLineProps{
                .textProvider = "Pen",
                .wifiStatusProvider = ctx.wifiStatusProvider,
                .onBackPress = [this]() {
                    if (router()) router()->popScreen();
                }
            }),

            std::make_unique<widgets::ScrollableVerticalLayout>(
                widgets::ScrollableVerticalLayoutStyle{},
                
                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "Up Angle",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .initialValue = static_cast<int>(ctx.runtimeSettings.penUpAngle_deg()),
                        .next = [](int current) { return std::min(current + 1, 180); },
                        .prev = [](int current) { return std::max(current - 1, 0); },
                        .onChange = [&sp = ctx.settingsPersistence](const int& newValue) {
                            sp.setPenUpAngle_deg(static_cast<float>(newValue));
                        },
                        .toString = [](int value) { return GlyphString(std::to_string(value)) + customChars::DegSymbol; }
                    }  
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "Down Angle",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .initialValue = static_cast<int>(ctx.runtimeSettings.penDownAngle_deg()),
                        .next = [](int current) { return std::min(current + 1, 180); },
                        .prev = [](int current) { return std::max(current - 1, 0); },
                        .onChange = [&sp = ctx.settingsPersistence](const int& newValue) {
                            sp.setPenDownAngle_deg(static_cast<float>(newValue));
                        },
                        .toString = [](int value) { return GlyphString(std::to_string(value)) + customChars::DegSymbol; }
                    }
                })
            )
        )
    )
    {}
};

} // namespace screens
} // namespace ui