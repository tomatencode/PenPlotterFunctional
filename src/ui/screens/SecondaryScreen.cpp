#include "SecondaryScreen.hpp"
#include "../framework/router/Router.hpp"
#include "../framework/widgets/Builder.hpp"

SecondaryScreen::SecondaryScreen()
    : Screen(
        ui::widgets::make_widget<HeaderLine>(
            "Settings",
            true,
            [this]() {
                if (router())
                    router()->popScreen();
            }
        )
    )
{
}
