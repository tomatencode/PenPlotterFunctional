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


FilesScreen::FilesScreen(JobManager& jobManager)
    : Screen(
        widgets::make_widget<widgets::VerticalLayout>(
            widgets::VerticalLayoutStyle{},
            widgets::make_widget<components::HeaderLine>("Files", true, [this]() {
                if (router()) {
                    router()->popScreen(); // Go back to the previous screen
                }
            }),
            widgets::make_widget<components::FileList>([this, &jobManager](const String& file) {
                FileDetailsScreen* detailsScreen = new FileDetailsScreen(file, jobManager);
                if (router()) {
                    router()->pushScreen(detailsScreen);
                }
            })
        )
    , 1) // Start with the file list focused
{
}

void FilesScreen::onEnter() {
    Screen::onEnter();
    reload(); // Refresh file list each time we enter the screen
}

} // namespace screens
} // namespace ui