#include "FilesScreen.hpp"

// Include related screens to enable navigation
#include "../framework/router/Router.hpp"

// Include components and widgets used in this screen
#include "../components/HeaderLine.hpp"
#include "../framework/widgets/Builder.hpp"
#include "../framework/widgets/leaves/ButtonWidget.hpp"
#include "../framework/widgets/leaves/LabelWidget.hpp"
#include "../framework/widgets/layouts/VerticalLayout.hpp"
#include "../framework/widgets/layouts/ScrollableVerticalLayout.hpp"

namespace ui {
namespace screens {

widgets::ButtonStyle fileButtonStyle = {
    .leftNormal = GLYPH_NONE,
    .rightNormal = GLYPH_NONE,
    .leftFocused = '>',
    .rightFocused = GLYPH_NONE,
    .leftPressed = '-',
    .rightPressed = GLYPH_NONE
};

FilesScreen::FilesScreen()
    : Screen(
        widgets::make_layout<widgets::VerticalLayout>(
            widgets::VerticalLayoutStyle{},
            widgets::make_widget<components::HeaderLine>("Files", true, [this]() {
                if (router()) {
                    router()->popScreen(); // Go back to the previous screen
                }
            }),
            widgets::make_layout<widgets::ScrollableVerticalLayout>(
                widgets::ScrollableVerticalLayoutStyle{},
                widgets::make_widget<widgets::ButtonWidget>(
                    widgets::make_widget<widgets::LabelWidget>("File 1"),
                    fileButtonStyle,
                    []() { /* Handle File 1 action */ }
                ),
                widgets::make_widget<widgets::ButtonWidget>(
                    widgets::make_widget<widgets::LabelWidget>("File 2"),
                    fileButtonStyle,
                    []() { /* Handle File 2 action */ }
                ),
                widgets::make_widget<widgets::ButtonWidget>(
                    widgets::make_widget<widgets::LabelWidget>("File 3"),
                    fileButtonStyle,
                    []() { /* Handle File 3 action */ }
                ),
                widgets::make_widget<widgets::ButtonWidget>(
                    widgets::make_widget<widgets::LabelWidget>("File 4"),
                    fileButtonStyle,
                    []() { /* Handle File 4 action */ }
                ),
                widgets::make_widget<widgets::ButtonWidget>(
                    widgets::make_widget<widgets::LabelWidget>("File 5"),
                    fileButtonStyle,
                    []() { /* Handle File 5 action */ }
                )
            )
        )
    )
{
}

} // namespace screens
} // namespace ui