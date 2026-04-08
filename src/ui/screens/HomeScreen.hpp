#pragma once

#include "../framework/screen/Screen.hpp"
#include "ScreensContext.hpp"

// Include related screens to enable navigation
#include "plotting/FilesScreen.hpp"
#include "manualControll/ManualControllScreen.hpp"
//#include "settings/SettingsScreen.hpp"
#include "../framework/router/Router.hpp"

// Include components and widgets used in this screen
#include "../components/HeaderLine.hpp"
#include "../framework/widgets/leaves/Button.hpp"
#include "../framework/widgets/leaves/Label.hpp"
#include "../framework/widgets/layouts/LinearLayout.hpp"

namespace ui {
namespace screens {

// The main screen of the UI
class HomeScreen : public Screen
{
public:
    HomeScreen(const ScreensContext& ctx)
    : Screen(
        std::make_unique<widgets::LinearLayout>(
            widgets::LinearLayoutStyle{.axis = widgets::Axis::Vertical, .horizontalAlign = widgets::HorizontalAlignment::Center},
            std::make_unique<components::HeaderLine>(components::HeaderLineProps{
                .textProvider = "Pen Plotter",
                .wifiStatusProvider = ctx.wifiStatusProvider
            }),

            std::make_unique<widgets::Button>(
                widgets::ButtonProps{
                    .onPress = [this, ctx]() {
                        if (router()) {
                            auto filesScreen = std::make_unique<FilesScreen>(ctx);
                            router()->pushScreen(std::move(filesScreen));
                        }
                    }
                },
                std::make_unique<widgets::Label>("Plot")
            ),

            std::make_unique<widgets::Button>(
                widgets::ButtonProps{
                    .onPress = [this, ctx]() {
                        if (router()) {
                            auto manualControlScreen = std::make_unique<ManualControllScreen>(ctx);
                            router()->pushScreen(std::move(manualControlScreen));
                        }
                    }
                },
                std::make_unique<widgets::Label>("Manual Control")
            )
            /*
            std::make_unique<widgets::Button>(
                widgets::ButtonProps{
                    .onPress = [this, ctx]() {
                        if (router()) {
                            auto settingsScreen = std::make_unique<SettingsScreen>(ctx);
                            router()->pushScreen(std::move(settingsScreen));
                        }
                    }
                },
                std::make_unique<widgets::Label>("Settings")
            )
            */
        )
    , 0) // Start with Plot button focused
    {}
};

} // namespace screens
} // namespace ui