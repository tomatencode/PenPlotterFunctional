#include "TestUI.hpp"
#include "SecondaryScreen.hpp"
#include "../framework/router/Router.hpp"

#include "../framework/widgets/Builder.hpp"
#include "../framework/widgets/leaves/ButtonWidget.hpp"
#include "../framework/widgets/leaves/LabelWidget.hpp"

using namespace ui;

widgets::LayoutStyle HomeCenterdLayoutStyle()
{
    widgets::LayoutStyle style;
    style.horizontalAlign = widgets::HorizontalAlignment::Center;
    return style;
}

TestScreen::TestScreen()
    : Screen(
        widgets::make_layout<widgets::VerticalLayout>(HomeCenterdLayoutStyle(),
            widgets::make_widget<components::HeaderLine>("Pen Plotter UI", false),
            widgets::make_widget<widgets::ButtonWidget>(
                widgets::make_widget<widgets::LabelWidget>("Next Screen"),
                widgets::ButtonStyle(),
                [this]() {
                    if (router()) {
                        static SecondaryScreen secondScreen;
                        router()->pushScreen(&secondScreen);
                    }
                }
            )
        )
    )
{
}