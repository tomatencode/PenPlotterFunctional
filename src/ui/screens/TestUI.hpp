#pragma once

#include "../framework/screen/Screen.hpp"
#include "../framework/widgets/leaves/LabelWidget.hpp"
#include "../framework/widgets/leaves/ButtonWidget.hpp"
#include "../framework/text/textSources/StaticText.hpp"
#include "../framework/text/textSources/FunctionText.hpp"

// A simple screen that demonstrates widget layout, focus, and button interaction.
class TestScreen : public Screen
{
public:
    TestScreen();

private:
    // UI elements stored as members to keep lifetime tied to the screen
    StaticText titleText;
    LabelWidget titleLabel;

    FunctionText toggleText;
    LabelWidget toggleLabel;
    ButtonWidget toggleButton;
};

// Convenience helper for getting a screen instance (optional).
Screen* createTestScreen();
