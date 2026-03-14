#include "TestUI.hpp"
#include "SecondaryScreen.hpp"
#include "../framework/router/Router.hpp"

#include "../framework/widgets/leaves/LabelWidget.hpp"
#include "../framework/widgets/leaves/ButtonWidget.hpp"
#include "../framework/text/textSources/StaticText.hpp"
#include "../framework/text/textSources/FunctionText.hpp"

static void onTogglePressed(void* ctx)
{
    if (ctx == nullptr) return;
    TestScreen* self = static_cast<TestScreen*>(ctx);

    // Push a secondary screen
    if (self->router())
    {
        static SecondaryScreen secondScreen;
        self->router()->pushScreen(&secondScreen);
    }
}

TestScreen::TestScreen()
    : Screen() // base initialized; we'll set root after member init
    , titleText("Pen Plotter UI")
    , titleLabel(titleText)
    , BtnText("Next Screen")
    , BtnLabel(BtnText)
    , Button(&BtnLabel, ButtonStyle(), onTogglePressed, nullptr, this)
{
    Widget* children[] = { &titleLabel, &Button };
    initRoot(children, 2);
}

Screen* createTestScreen()
{
    static TestScreen instance;
    return &instance;
}
