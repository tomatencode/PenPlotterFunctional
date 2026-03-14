#include "TestUI.hpp"

#include "../framework/widgets/leaves/LabelWidget.hpp"
#include "../framework/widgets/leaves/ButtonWidget.hpp"
#include "../framework/text/textSources/StaticText.hpp"
#include "../framework/text/textSources/FunctionText.hpp"

// Simple shared state for the test screen
static bool g_buttonToggled = false;

static const char* getToggleText()
{
    return g_buttonToggled ? "Toggled: ON" : "Toggled: OFF";
}

static void onTogglePressed()
{
    g_buttonToggled = !g_buttonToggled;
}

TestScreen::TestScreen()
    : Screen() // base initialized; we'll set root after member init
    , titleText("Pen Plotter UI")
    , titleLabel(titleText)
    , toggleText(getToggleText)
    , toggleLabel(toggleText)
    , toggleButton(&toggleLabel, ButtonStyle(), onTogglePressed)
{
    Widget* children[] = { &titleLabel, &toggleButton };
    initRoot(children, 2);
}

Screen* createTestScreen()
{
    static TestScreen instance;
    return &instance;
}
