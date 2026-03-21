#include "HomeScreen.hpp"

// Include related screens to enable navigation
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

HomeScreen::HomeScreen(JobManager& jobManager)
    : Screen(
        widgets::make_layout<widgets::VerticalLayout>(
            widgets::VerticalLayoutStyle{.horizontalAlign = widgets::HorizontalAlignment::Center},
            widgets::make_widget<components::HeaderLine>("Pen Plotter", false),

            widgets::make_widget<widgets::ButtonWidget>(
                widgets::make_widget<widgets::LabelWidget>("Plot"),
                widgets::ButtonStyle(),
                [this, &jobManager]() {
                    if (router()) {
                        static FilesScreen* filesScreen = new FilesScreen(jobManager);
                        router()->pushScreen(filesScreen);
                    }
                }
            ),

            widgets::make_widget<widgets::ButtonWidget>(
                widgets::make_widget<widgets::LabelWidget>("Settings"),
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