#pragma once

#include "../framework/screen/Screen.hpp"

#include "systemServices/MotionState.hpp"
#include "jobController/JobController.hpp"

#include "storage/FileObserver.hpp"
#include "storage/FileManager.hpp"

// Include related screens to enable navigation
#include "../framework/router/Router.hpp"
#include "FileDetailsScreen.hpp"

// Include components and widgets used in this screen
#include "../components/HeaderLine.hpp"
#include "../components/FileList.hpp"
#include "../framework/widgets/Builder.hpp"
#include "../framework/widgets/leaves/Button.hpp"
#include "../framework/widgets/leaves/Label.hpp"
#include "../framework/widgets/layouts/VerticalLayout.hpp"
#include "../framework/widgets/layouts/ScrollableVerticalLayout.hpp"

namespace ui {
namespace screens {

class FilesScreen : public Screen, public FileObserver
{
public:
    FilesScreen(JobController& jobController,
                MotionState& motionState,
                FileManager& fileManager,
                std::function<bool()> wifiStatusProvider
            )
    : Screen(
        widgets::make_widget<widgets::VerticalLayout>(
            widgets::LinearLayoutStyle{},
            widgets::make_widget<components::HeaderLine>("Files", wifiStatusProvider, [this]() {
                if (router()) {
                    router()->popScreen();
                }
            }),
            widgets::make_widget<components::FileList>(fileManager, [this, &jobController, &motionState, &fileManager, wifiStatusProvider](const String& file) {
                auto detailsScreen = std::make_unique<FileDetailsScreen>(file, jobController, motionState, fileManager, wifiStatusProvider);
                if (router()) {
                    router()->pushScreen(std::move(detailsScreen));
                }
            })
        )
    , 1), _fileManager(fileManager)
    {
        fileManager.registerFileObserver(this);
    }

    ~FilesScreen() {
        _fileManager.unregisterFileObserver(this);
    }

    void onFileEvent(FileEvent event, const String& path) override {
        if (event == FileEvent::ADDED || event == FileEvent::REMOVED) {
            reload();
        }
    }

private:
    FileManager& _fileManager;
};

} // namespace screens
} // namespace ui