#include "PlottingScreen.hpp"

// Include related screens to enable navigation
#include "FilesScreen.hpp"

// Include related screens to enable navigation
#include "../framework/router/Router.hpp"

// Include components and widgets used in this screen
#include "../components/HeaderLine.hpp"
#include "../framework/widgets/Builder.hpp"
#include "../framework/widgets/leaves/ButtonWidget.hpp"
#include "../framework/widgets/leaves/LabelWidget.hpp"
#include "../framework/widgets/leaves/ProgressBarWidget.hpp"

namespace ui {
namespace screens {

PlottingScreen::PlottingScreen(const String& filename)
    : Screen(
        widgets::make_layout<widgets::VerticalLayout>(
            widgets::VerticalLayoutStyle{.horizontalAlign = widgets::HorizontalAlignment::Center},
            widgets::make_widget<components::HeaderLine>(filename.substring(0, filename.length() - 6).c_str(), false),

            widgets::make_widget<widgets::ProgressBarWidget>(widgets::ProgressBarStyle{}, []() { return 50; }), // Placeholder for actual progress

            widgets::make_layout<widgets::HorizontalLayout>(
                widgets::HorizontalLayoutStyle{.spacingMode = widgets::SpacingMode::Even},
                widgets::make_widget<widgets::LabelWidget>("-2:00")
            ),

            widgets::make_layout<widgets::HorizontalLayout>(
                    widgets::HorizontalLayoutStyle{.spacingMode = widgets::SpacingMode::SpaceAround},

                    widgets::make_widget<widgets::ButtonWidget>(
                        widgets::make_widget<widgets::LabelWidget>("Pause"),
                        widgets::ButtonStyle(),
                        [filename]() {
                            // Handle plot file action
                        }
                    ),
                    widgets::make_widget<widgets::ButtonWidget>(
                        widgets::make_widget<widgets::LabelWidget>("Abort"),
                        widgets::ButtonStyle(),
                        [filename]() {
                            // Handle abort plot action
                        }
                    )
                )
            )
        )
    
{
}

} // namespace screens
} // namespace ui