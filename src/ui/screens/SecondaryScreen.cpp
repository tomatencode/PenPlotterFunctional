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
    , titleText("Second Screen")
    , titleLabel(titleText)
    , backText("Back")
    , backLabel(backText)
    , backButton(&backLabel, ButtonStyle(), onBackPressed, nullptr, this)
{
    Widget* children[] = { &titleLabel, &backButton };
    initRoot(children, 2);
}
