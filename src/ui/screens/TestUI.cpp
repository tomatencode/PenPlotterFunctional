#include "TestUI.hpp"
#include "SecondaryScreen.hpp"
#include "../framework/router/Router.hpp"

#include "../framework/widgets/leaves/LabelWidget.hpp"
#include "../framework/widgets/leaves/ButtonWidget.hpp"

TestScreen::TestScreen()
    : Screen() // base initialized; we'll set root after member init
    , titleLabel("Pen Plotter UI")
    , BtnLabel("Next Screen")
    , Button(&BtnLabel, ButtonStyle(), [this]() {
        if (router()) {
            static SecondaryScreen secondScreen;
            router()->pushScreen(&secondScreen);
        }
      })
{
    Widget* children[] = { &titleLabel, &Button };
    initRoot(children, 2);
}

Screen* createTestScreen()
{
    static TestScreen instance;
    return &instance;
}
