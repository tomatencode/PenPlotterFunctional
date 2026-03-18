#include "SecondaryScreen.hpp"
#include "../framework/router/Router.hpp"

SecondaryScreen::SecondaryScreen()
    : Screen()
    , header("Settings", true, [this]() {
        if (router())
            router()->popScreen();
      })
{
    initRoot(&header);
}
