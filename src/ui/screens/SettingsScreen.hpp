#pragma once

#include <functional>

#include "../framework/screen/Screen.hpp"

#include "systemServices/MotionState.hpp"
#include "jobController/JobController.hpp"
#include "storage/FileManager.hpp"

// Include related screens to enable navigation
#include "FilesScreen.hpp"
#include "../framework/router/Router.hpp"

// Include components and widgets used in this screen
#include "../components/HeaderLine.hpp"
#include "../components/ColabsibleElement.hpp"
#include "../framework/widgets/leaves/Button.hpp"
#include "../framework/widgets/leaves/Label.hpp"
#include "../framework/widgets/layouts/LinearLayout.hpp"
#include "../framework/widgets/layouts/ScrollableVerticalLayout.hpp"


namespace ui {
namespace screens {

class SettingsScreen : public Screen
{
public:
    SettingsScreen(FileManager& fileManager,
                   std::function<bool()> wifiStatusProvider
                  )
    : Screen(
        std::make_unique<widgets::ScrollableVerticalLayout>(
            widgets::ScrollableVerticalLayoutStyle{},

            std::make_unique<components::HeaderLine>(
                "Settings",
                wifiStatusProvider,
                [this]() {
                    if (router()) {
                        router()->popScreen();
                    }
                }
            ),

            std::make_unique<components::CollabsibleElement>(
                components::CollabsibleElementProps{
                    .label = "Wifi Settings"
                },
                std::make_unique<widgets::LinearLayout>(
                    widgets::LinearLayoutStyle{.axis = widgets::Axis::Vertical},
                    std::make_unique<widgets::Label>("Wifi settings would go here"),
                    std::make_unique<widgets::Label>("...")
                )
            ),

            std::make_unique<components::CollabsibleElement>(
                components::CollabsibleElementProps{
                    .label = "PLotting Settings"
                },
                std::make_unique<widgets::LinearLayout>(
                    widgets::LinearLayoutStyle{.axis = widgets::Axis::Vertical},
                    std::make_unique<widgets::Label>("Plotting settings would go here"),
                    std::make_unique<widgets::Label>("...")
                )
            )
        )
    )
    {}
};

} // namespace screens
} // namespace ui