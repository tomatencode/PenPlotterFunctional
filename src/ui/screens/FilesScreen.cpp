#include "FilesScreen.hpp"

// Include related screens to enable navigation
#include "../framework/router/Router.hpp"

// Include components and widgets used in this screen
#include "../components/HeaderLine.hpp"
#include "../framework/widgets/Builder.hpp"
#include "../framework/widgets/leaves/ButtonWidget.hpp"
#include "../framework/widgets/leaves/LabelWidget.hpp"

namespace ui {
namespace screens {

widgets::VerticalLayoutStyle FilesLayout()
{
    widgets::VerticalLayoutStyle style;
    style.horizontalAlign = widgets::HorizontalAlignment::Center;
    return style;
}

FilesScreen::FilesScreen()
    : Screen(
        widgets::make_widget<widgets::VerticalLayout>(
        FilesLayout(),
        std::vector<std::unique_ptr<widgets::Widget>>{

            // Header with title and WiFi status - includes a back button
            widgets::make_widget<components::HeaderLine>("Files", true, [this]() {
                if (router()) {
                    router()->popScreen(); // Go back to the previous screen
                }
            })
            
        }
        )
    )
{
}

} // namespace screens
} // namespace ui