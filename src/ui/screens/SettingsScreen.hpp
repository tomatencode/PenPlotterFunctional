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
#include "../framework/widgets/leaves/ValueSelector.hpp"
#include "../framework/widgets/layouts/LinearLayout.hpp"

namespace ui {
namespace screens {

class SettingsScreen : public Screen
{
public:
    SettingsScreen(FileManager& fileManager,
                   std::function<bool()> wifiStatusProvider
                  )
    : Screen(
        std::make_unique<widgets::LinearLayout>(
            widgets::LinearLayoutStyle{.axis = widgets::Axis::Vertical},

            std::make_unique<components::HeaderLine>(
                "Settings",
                wifiStatusProvider,
                [this]() {
                    if (router()) {
                        router()->popScreen();
                    }
                }
            ),

            std::make_unique<widgets::ScrollableVerticalLayout>(
                widgets::ScrollableVerticalLayoutStyle{},
                std::make_unique<components::CollabsibleElement>(
                    components::CollabsibleElementProps{
                        .label = "Wifi Settings"
                    },
                    std::make_unique<widgets::LinearLayout>(
                        widgets::LinearLayoutStyle{.axis = widgets::Axis::Horizontal},
                        std::make_unique<widgets::Label>("Wifi: "),

                        std::make_unique<widgets::ValueSelector<bool>>(
                            widgets::ValueSelectorProps<bool>{
                                .initialValue = false,
                                .next = [](const bool& value) { return !value; },
                                .prev = [](const bool& value) { return !value; },
                                .toString = [](const bool& value) { return value ? "On" : "Off"; },
                            }
                        )
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
    )
    {}
};

} // namespace screens
} // namespace ui