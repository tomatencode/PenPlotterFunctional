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
#include "../framework/widgets/leaves/Button.hpp"
#include "../framework/widgets/leaves/Label.hpp"
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
            widgets::LinearLayoutStyle{.axis = widgets::Axis::Vertical, .horizontalAlign = widgets::HorizontalAlignment::Center},

            std::make_unique<components::HeaderLine>(
                "Settings",
                wifiStatusProvider,
                [this]() {
                    if (router()) {
                        router()->popScreen();
                    }
                }
            ),

            std::make_unique<widgets::LinearLayout>(
                widgets::LinearLayoutStyle{.axis = widgets::Axis::Horizontal},

                std::make_unique<widgets::ProgressBar>(widgets::ProgressBarProps{
                    .getProgress = []() { return 1.0; }
                }),

                std::make_unique<widgets::ProgressBar>(widgets::ProgressBarProps{
                    .getProgress = []() { return 1.0; }
                })
            )
        )
    )
    {}
};

} // namespace screens
} // namespace ui