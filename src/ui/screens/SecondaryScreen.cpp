#include "SecondaryScreen.hpp"
#include "../framework/router/Router.hpp"
#include "../framework/widgets/Builder.hpp"
#include "../framework/widgets/leaves/ValueSelector.hpp"

using namespace ui;


widgets::LayoutStyle CenterdLayoutStyle()
{
    widgets::LayoutStyle style;
    style.horizontalAlign = widgets::HorizontalAlignment::Center;
    return style;
}

SecondaryScreen::SecondaryScreen()
    : Screen(
        widgets::make_layout<widgets::VerticalLayout>(CenterdLayoutStyle(),
            widgets::make_widget<components::HeaderLine>(
                "Settings",
                true,
                [this]() {
                    if (router())
                        router()->popScreen();
                }
            ),
            widgets::make_layout<widgets::HorizontalLayout>(widgets::LayoutStyle(),
                widgets::make_widget<widgets::LabelWidget>("Value: "),
                widgets::make_widget<widgets::ValueSelector<int>>(5,
                    [](int val) { return val + 1; }, // next value (stub)
                    [](int val) { return val - 1; }, // prev value (stub)
                    [](const int& newValue) { /* onChange callback (stub) */ },
                    [](const int& val) { return std::to_string(val); } // ToString function
                )
            )

        )
    )
{
}
