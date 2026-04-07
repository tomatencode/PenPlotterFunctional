#pragma once

#include "ui/framework/screen/Screen.hpp"
#include "ui/framework/router/Router.hpp"
#include "ui/components/HeaderLine.hpp"
#include "ui/components/LabeledTextEditor.hpp"
#include "ui/framework/widgets/leaves/Button.hpp"
#include "ui/framework/widgets/leaves/Label.hpp"
#include "ui/framework/widgets/layouts/LinearLayout.hpp"
#include "ui/framework/widgets/layouts/ScrollableVerticalLayout.hpp"
#include "ui/screens/ScreensContext.hpp"

namespace ui {
namespace screens {

class WifiSettingsScreen : public Screen
{
public:
    WifiSettingsScreen(const ScreensContext& ctx)
    : Screen(
        std::make_unique<widgets::LinearLayout>(
            widgets::LinearLayoutStyle{.axis = widgets::Axis::Vertical},

            std::make_unique<components::HeaderLine>(components::HeaderLineProps{
                .textProvider = "Wifi",
                .wifiStatusProvider = ctx.wifiStatusProvider,
                .onBackPress = [this]() {
                    if (router()) router()->popScreen();
                }
            }),

            std::make_unique<widgets::ScrollableVerticalLayout>(
                widgets::ScrollableVerticalLayoutStyle{},

                std::make_unique<components::LabeledTextEditor<std::string>>(
                    components::LabeledTextEditorProps<std::string>{
                        .labelText = "SSID",
                        .textEditorProps = widgets::TextEditorProps{
                            .initialText = ctx.runtimeSettings.getSSID(),
                            .onChange = [&sp = ctx.settingsPersistence](const std::string& newValue) {
                                sp.setSSID(newValue);
                            }
                        }
                    }
                ),

                std::make_unique<components::LabeledTextEditor<std::string>>(
                    components::LabeledTextEditorProps<std::string>{
                        .labelText = "Password",
                        .textEditorProps = widgets::TextEditorProps{
                            .initialText = ctx.runtimeSettings.getPassword(),
                            .onChange = [&sp = ctx.settingsPersistence](const std::string& newValue) {
                                sp.setPassword(newValue);
                            }
                        }
                    }
                ),

                std::make_unique<components::LabeledTextEditor<std::string>>(
                    components::LabeledTextEditorProps<std::string>{
                        .labelText = "mDNS Name",
                        .textEditorProps = widgets::TextEditorProps{
                            .initialText = ctx.runtimeSettings.getMdnsName(),
                            .onChange = [&sp = ctx.settingsPersistence](const std::string& newValue) {
                                sp.setMdnsName(newValue);
                            }
                        }
                    }
                )
            )
        )
    )
    {}
};

} // namespace screens
} // namespace ui