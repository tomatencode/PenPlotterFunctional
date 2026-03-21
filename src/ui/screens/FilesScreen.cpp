#include "FilesScreen.hpp"

// Include related screens to enable navigation
#include "../framework/router/Router.hpp"
#include "FileDetailsScreen.hpp"

// Include components and widgets used in this screen
#include "../components/HeaderLine.hpp"
#include "../components/FileList.hpp"
#include "../framework/widgets/Builder.hpp"
#include "../framework/widgets/leaves/ButtonWidget.hpp"
#include "../framework/widgets/leaves/LabelWidget.hpp"
#include "../framework/widgets/layouts/VerticalLayout.hpp"
#include "../framework/widgets/layouts/ScrollableVerticalLayout.hpp"

namespace ui {
namespace screens {


FilesScreen::FilesScreen()
    : Screen(
        widgets::make_layout<widgets::VerticalLayout>(
            widgets::VerticalLayoutStyle{},
            widgets::make_widget<components::HeaderLine>("Files", true, [this]() {
                if (router()) {
                    router()->popScreen(); // Go back to the previous screen
                }
            }),
            widgets::make_widget<components::FileList>([this](const String& file) {
                FileDetailsScreen* detailsScreen = new FileDetailsScreen(file);
                if (router()) {
                    router()->pushScreen(detailsScreen);
                }
            })
        )
    )
{
}

} // namespace screens
} // namespace ui