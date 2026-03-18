#include "SecondaryScreen.hpp"
#include "../framework/router/Router.hpp"

SecondaryScreen::SecondaryScreen()
    : Screen()
    , header(nullptr)
{
    header = new HeaderLine("Settings", true, [this]() {
        if (router())
            router()->popScreen();
      });

    initRoot(std::unique_ptr<Widget>(header));
}
