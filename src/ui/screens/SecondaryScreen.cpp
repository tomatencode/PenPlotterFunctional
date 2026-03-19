#include "SecondaryScreen.hpp"
#include "../framework/router/Router.hpp"
#include "../framework/widgets/Builder.hpp"

using namespace ui;

SecondaryScreen::SecondaryScreen()
    : Screen(
        widgets::make_widget<components::HeaderLine>(
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
