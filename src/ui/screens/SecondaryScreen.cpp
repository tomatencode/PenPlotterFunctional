#include "SecondaryScreen.hpp"
#include "../framework/router/Router.hpp"

static void onBackPressed(void* ctx)
{
    if (ctx == nullptr) return;
    SecondaryScreen* self = static_cast<SecondaryScreen*>(ctx);
    if (self->router())
        self->router()->popScreen();
}

SecondaryScreen::SecondaryScreen()
    : Screen()
    , header("Settings", true, onBackPressed, this)
{
    initRoot(&header);
}
