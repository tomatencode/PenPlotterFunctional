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

HomeScreen::HomeScreen(JobManager& jobManager, MotionState& ms, FileManager& fileManager)
    : Screen(
        widgets::make_widget<widgets::VerticalLayout>(
            widgets::VerticalLayoutStyle{.horizontalAlign = widgets::HorizontalAlignment::Center},
            widgets::make_widget<components::HeaderLine>("Pen Plotter", false),

            widgets::make_widget<widgets::Button>(
                "Plot",
                widgets::ButtonStyle(),
                [this, &jobManager, &ms, &fileManager]() {
                    if (router()) {
                        FilesScreen* filesScreen = new FilesScreen(jobManager, ms, fileManager);
                        router()->pushScreen(filesScreen);
                    }
                }
            ),

            widgets::make_widget<widgets::Button>(
                "Settings",
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