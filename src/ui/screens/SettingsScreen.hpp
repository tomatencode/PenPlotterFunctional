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

class SettingsScreen : public Screen
{
public:
    SettingsScreen(FileManager& fileManager,
                   std::function<bool()> wifiStatusProvider
                  )
    : Screen(
        widgets::make_widget<widgets::VerticalLayout>(
            widgets::VerticalLayoutStyle{.horizontalAlign = widgets::HorizontalAlignment::Center},

            widgets::make_widget<components::HeaderLine>(
                "Settings",
                wifiStatusProvider,
                [this]() {
                    if (router()) {
                        router()->popScreen();
                    }
                }
            )
        )
    )
    {}
};

} // namespace screens
} // namespace ui