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
#include "ui/components/LabeledTextEditor.hpp"
#include "ui/framework/widgets/leaves/Button.hpp"
#include "ui/framework/widgets/leaves/Label.hpp"
#include "ui/framework/widgets/layouts/LinearLayout.hpp"
#include "ui/framework/widgets/layouts/ScrollableVerticalLayout.hpp"

namespace ui {
namespace screens {

class WifiSettingsScreen : public Screen
{
public:
    WifiSettingsScreen(std::function<bool()> wifiStatusProvider, SettingPersistence& settingsPersistence, RuntimeSettings& runtimeSettings)
    : Screen(
        std::make_unique<widgets::LinearLayout>(
            widgets::LinearLayoutStyle{.axis = widgets::Axis::Vertical},

            std::make_unique<components::HeaderLine>(components::HeaderLineProps{
                .textProvider = "Wifi",
                .wifiStatusProvider = wifiStatusProvider,
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
                            .initialText = runtimeSettings.getSSID(),
                            .onChange = [&settingsPersistence](const std::string& newValue) {
                                settingsPersistence.setSSID(newValue);
                            }
                        }
                    }
                ),

                std::make_unique<components::LabeledTextEditor<std::string>>(
                    components::LabeledTextEditorProps<std::string>{
                        .labelText = "Password",
                        .textEditorProps = widgets::TextEditorProps{
                            .initialText = runtimeSettings.getPassword(),
                            .onChange = [&settingsPersistence](const std::string& newValue) {
                                settingsPersistence.setPassword(newValue);
                            }
                        }
                    }
                ),

                std::make_unique<components::LabeledTextEditor<std::string>>(
                    components::LabeledTextEditorProps<std::string>{
                        .labelText = "mDNS Name",
                        .textEditorProps = widgets::TextEditorProps{
                            .initialText = runtimeSettings.getMdnsName(),
                            .onChange = [&settingsPersistence](const std::string& newValue) {
                                settingsPersistence.setMdnsName(newValue);
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