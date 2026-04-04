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

            std::make_unique<components::HeaderLine>(
                "Wifi",
                wifiStatusProvider,
                [this]() {
                    if (router()) {
                        router()->popScreen();
                    }
                }
            ),

            std::make_unique<widgets::ScrollableVerticalLayout>(
                widgets::ScrollableVerticalLayoutStyle{},
                
                std::make_unique<widgets::Label>("Configure Wifi")
            )
        )
    )
    {}
};

} // namespace screens
} // namespace ui