#pragma once

#include "../framework/screen/Screen.hpp"
#include "../framework/widgets/leaves/LabelWidget.hpp"
#include "../framework/widgets/leaves/ButtonWidget.hpp"
#include "../framework/text/textSources/StaticText.hpp"

// Forward declare secondary screen to avoid including it here.
class SecondaryScreen;

// A simple screen that demonstrates widget layout, focus, and button interaction.
class TestScreen : public Screen
{
public:
    TestScreen();

private:
    // UI elements stored as members to keep lifetime tied to the screen
    StaticText titleText;
    LabelWidget titleLabel;

    StaticText BtnText;
    LabelWidget BtnLabel;
    ButtonWidget Button;
};

// Convenience helper for getting a screen instance (optional).
Screen* createTestScreen();
