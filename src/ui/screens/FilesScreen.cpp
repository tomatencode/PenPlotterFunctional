#include "FilesScreen.hpp"

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


FilesScreen::FilesScreen(JobController& jobController, MotionState& motionState, FileManager& fileManager, std::function<bool()> wifiStatusProvider)
    : Screen(
        widgets::make_widget<widgets::VerticalLayout>(
            widgets::VerticalLayoutStyle{},
            widgets::make_widget<components::HeaderLine>("Files", wifiStatusProvider, [this]() {
                if (router()) {
                    router()->popScreen(); // Go back to the previous screen
                }
            }),
            widgets::make_widget<components::FileList>(fileManager, [this, &jobController, &motionState, &fileManager, wifiStatusProvider](const String& file) {
                FileDetailsScreen* detailsScreen = new FileDetailsScreen(file, jobController, motionState, fileManager, wifiStatusProvider);
                if (router()) {
                    router()->pushScreen(detailsScreen);
                }
            })
        )
    , 1), _fileManager(fileManager)
{
    fileManager.registerFileObserver(this);
}

FilesScreen::~FilesScreen() {
    _fileManager.unregisterFileObserver(this);
}

void FilesScreen::onFileEvent(FileEvent event, const String& path) {
    if (event == FileEvent::ADDED || event == FileEvent::REMOVED) {
        reload();
    }
}

} // namespace screens
} // namespace ui