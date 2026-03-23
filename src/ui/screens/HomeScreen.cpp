#include "HomeScreen.hpp"

// Include related screens to enable navigation
#include "FilesScreen.hpp"

// Include related screens to enable navigation
#include "../framework/router/Router.hpp"

// Include components and widgets used in this screen
#include "../components/HeaderLine.hpp"
#include "../framework/widgets/Builder.hpp"
#include "../framework/widgets/leaves/Button.hpp"
#include "../framework/widgets/leaves/Label.hpp"

namespace ui {
namespace screens {

HomeScreen::HomeScreen(JobManager& jobManager)
    : Screen(
        widgets::make_widget<widgets::VerticalLayout>(
            widgets::VerticalLayoutStyle{.horizontalAlign = widgets::HorizontalAlignment::Center},
            widgets::make_widget<components::HeaderLine>("Pen Plotter", false),

            widgets::make_widget<widgets::Button>(
                widgets::make_widget<widgets::Label>("Plot"),
                widgets::ButtonStyle(),
                [this, &jobManager]() {
                    if (router()) {
                        FilesScreen* filesScreen = new FilesScreen(jobManager);
                        router()->pushScreen(filesScreen);
                    }
                }
            ),

            widgets::make_widget<widgets::Button>(
                widgets::make_widget<widgets::Label>("Settings"),
                widgets::ButtonStyle(),
                [this]() {
                    if (router()) {
                        //router()->pushScreen(...); // Navigate to settings screen (not implemented yet)
                    }
                }
            )
        )
    )
{
}

} // namespace screens
} // namespace ui