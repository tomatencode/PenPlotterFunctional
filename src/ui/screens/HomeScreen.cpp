#include "HomeScreen.hpp"

// Include related screens to enable navigation
#include "../framework/router/Router.hpp"

// Include components and widgets used in this screen
#include "../components/HeaderLine.hpp"
#include "../framework/widgets/Builder.hpp"
#include "../framework/widgets/leaves/ButtonWidget.hpp"
#include "../framework/widgets/leaves/LabelWidget.hpp"

namespace ui {
namespace screens {

widgets::LayoutStyle HomeLayout()
{
    widgets::LayoutStyle style;
    style.horizontalAlign = widgets::HorizontalAlignment::Center;
    return style;
}

HomeScreen::HomeScreen()
    : Screen(
        widgets::make_layout<widgets::VerticalLayout>(HomeLayout(),
        
            // Header with title and WiFi status
            widgets::make_widget<components::HeaderLine>("Pen Plotter", false),

            // Plot button
            widgets::make_widget<widgets::ButtonWidget>(
                widgets::make_widget<widgets::LabelWidget>("Plot"),
                widgets::ButtonStyle(),
                [this]() {
                    if (router()) {
                        //router()->pushScreen(...); // Navigate to plotting screen (not implemented yet)
                    }
                }
            ),

            // Settings button
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