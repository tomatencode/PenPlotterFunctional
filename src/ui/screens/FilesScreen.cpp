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
    style.spacingMode = widgets::SpacingMode::Fixed;
    style.spacing = 0;
    style.marginLeft = 0;
    style.marginRight = 0;
    style.marginTop = 0;
    style.marginBottom = 0;
    style.horizontalAlign = widgets::HorizontalAlignment::Center;
    return style;
}

FilesScreen::FilesScreen()
    : Screen(
        widgets::make_layout<widgets::VerticalLayout>(
            FilesLayout(),
            widgets::make_widget<components::HeaderLine>("Files", true, [this]() {
                if (router()) {
                    router()->popScreen(); // Go back to the previous screen
                }
            })
        )
    )
{
}

} // namespace screens
} // namespace ui