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
#include "../framework/widgets/Builder.hpp"
#include "../framework/widgets/leaves/Button.hpp"
#include "../framework/widgets/leaves/Label.hpp"
#include "../framework/widgets/layouts/VerticalLayout.hpp"

namespace ui {
namespace screens {

// The main screen of the UI
class HomeScreen : public Screen
{
public:
    HomeScreen(JobController& jobController,
               MotionState& motionState,
               FileManager& fileManager,
               std::function<bool()> wifiStatusProvider
               )
    : Screen(
        widgets::make_widget<widgets::VerticalLayout>(
            widgets::VerticalLayoutStyle{.horizontalAlign = widgets::HorizontalAlignment::Center},
            widgets::make_widget<components::HeaderLine>("Pen Plotter", wifiStatusProvider),

            widgets::make_widget<widgets::Button>(
                "Plot",
                widgets::ButtonStyle(),
                [this, &jobController, &motionState, &fileManager, wifiStatusProvider]() {
                    if (router()) {
                        auto filesScreen = std::make_unique<screens::FilesScreen>(jobController, motionState, fileManager, wifiStatusProvider);
                        router()->pushScreen(std::move(filesScreen));
                    }
                }
            ),

            widgets::make_widget<widgets::Button>(
                "Settings",
                widgets::ButtonStyle(),
                [this]() {
                    if (router()) {
                        //router()->pushScreen(...);
                    }
                }
            )
        )
    )
    {}
};

} // namespace screens
} // namespace ui