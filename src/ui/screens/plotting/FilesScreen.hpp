#pragma once

#include "ui/framework/screen/Screen.hpp"

#include "rtos/MotionState.hpp"
#include "jobController/JobController.hpp"

#include "storage/FileObserver.hpp"
#include "storage/FileManager.hpp"

// Include related screens to enable navigation
#include "ui/framework/router/Router.hpp"
#include "FileDetailsScreen.hpp"

// Include components and widgets used in this screen
#include "ui/components/HeaderLine.hpp"
#include "ui/components/FileList.hpp"
#include "ui/framework/widgets/leaves/Button.hpp"
#include "ui/framework/widgets/leaves/Label.hpp"
#include "ui/framework/widgets/layouts/LinearLayout.hpp"
#include "ui/framework/widgets/layouts/ScrollableVerticalLayout.hpp"

namespace ui {
namespace screens {

class FilesScreen : public Screen
{
public:
    FilesScreen(JobController& jobController,
                MotionState& motionState,
                FileManager& fileManager,
                std::function<bool()> wifiStatusProvider
            )
    : Screen(
        std::make_unique<widgets::LinearLayout>(
            widgets::LinearLayoutStyle{.axis = widgets::Axis::Vertical},
            std::make_unique<components::HeaderLine>("Files", wifiStatusProvider, [this]() {
                if (router()) {
                    router()->popScreen();
                }
            }),
            
            std::make_unique<components::FileList>(fileManager,
                [this, &jobController, &motionState, &fileManager, wifiStatusProvider](const std::string& file) {
                auto detailsScreen = std::make_unique<FileDetailsScreen>(file, jobController, motionState, fileManager, wifiStatusProvider);
                if (router()) {
                    router()->pushScreen(std::move(detailsScreen));
                }
            })
        )
    , 1)
    {}
};

} // namespace screens
} // namespace ui