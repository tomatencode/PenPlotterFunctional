#pragma once

#include "ui/framework/screen/Screen.hpp"
#include "ui/framework/router/Router.hpp"
#include "ui/components/HeaderLine.hpp"
#include "ui/framework/widgets/leaves/Button.hpp"
#include "ui/framework/widgets/leaves/Label.hpp"
#include "ui/framework/widgets/leaves/TextEditor.hpp"
#include "ui/framework/widgets/layouts/LinearLayout.hpp"
#include "ui/framework/widgets/layouts/ScrollableVerticalLayout.hpp"
#include "ui/screens/ScreensContext.hpp"

namespace ui {
namespace screens {

class NameSettingScreen : public Screen
{
public:
    NameSettingScreen(const ScreensContext& ctx)
    : Screen(
        std::make_unique<widgets::LinearLayout>(
            widgets::LinearLayoutStyle{.axis = widgets::Axis::Vertical},

            std::make_unique<components::HeaderLine>(components::HeaderLineProps{
                .textProvider = "Name & DNS",
                .wifiStatusProvider = ctx.wifiStatusProvider,
                .onBackPress = [this]() {
                    if (router()) router()->popScreen();
                }
            }),

            std::make_unique<widgets::LinearLayout>(
                widgets::LinearLayoutStyle{
                    .axis = widgets::Axis::Vertical,
                    .spacingMode = widgets::SpacingMode::Even
                },

                std::make_unique<widgets::LinearLayout>(
                    widgets::LinearLayoutStyle{
                        .axis = widgets::Axis::Horizontal,
                        .spacingMode = widgets::SpacingMode::Even
                    },
                    std::make_unique<widgets::Label>("Device Name"),

                    std::make_unique<widgets::TextEditor>(
                        widgets::TextEditorProps{
                            .initialText = ctx.runtimeSettings.name(),
                            .onChange = [&sp = ctx.settingsPersistence](const std::string& newValue) {
                                sp.setName(newValue);
                            }
                        }
                    )
                )
            )
        )
    )
    {}
};

} // namespace screens
} // namespace ui