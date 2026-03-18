#include "SecondaryScreen.hpp"
#include "../framework/router/Router.hpp"
#include "../framework/widgets/Builder.hpp"

SecondaryScreen::SecondaryScreen()
    : Screen(
        ui::widgets::make_widget<components::HeaderLine>(
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
