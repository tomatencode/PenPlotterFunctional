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

class PenServoScreen : public Screen
{
public:
    PenServoScreen(const ScreensContext& ctx)
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
                        .getValue = [&rs = ctx.runtimeSettings]() { return static_cast<int>(rs.penUpAngle_deg()); },
                        .next = [&sp = ctx.settingsPersistence](int v) { sp.setPenUpAngle_deg(static_cast<float>(v + 1)); },
                        .prev = [&sp = ctx.settingsPersistence](int v) { sp.setPenUpAngle_deg(static_cast<float>(v - 1)); },
                        .toString = [](const int& value) { return GlyphString(std::to_string(value)) + customChars::DegSymbol; }
                    }  
                }),

                std::make_unique<components::LabeledValueSelector<int>>(components::LabeledValueSelectorProps<int>{
                    .labelText = "Down Angle",
                    .valueSelectorProps = widgets::ValueSelectorProps<int>{
                        .getValue = [&rs = ctx.runtimeSettings]() { return static_cast<int>(rs.penDownAngle_deg()); },
                        .next = [&sp = ctx.settingsPersistence](int v) { sp.setPenDownAngle_deg(static_cast<float>(v + 1)); },
                        .prev = [&sp = ctx.settingsPersistence](int v) { sp.setPenDownAngle_deg(static_cast<float>(v - 1)); },
                        .toString = [](const int& value) { return GlyphString(std::to_string(value)) + customChars::DegSymbol; }
                    }
                })
            )
        )
    )
    {}
};

} // namespace screens
} // namespace ui