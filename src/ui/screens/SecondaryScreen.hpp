#pragma once

#include "../framework/screen/Screen.hpp"
#include "../framework/widgets/leaves/LabelWidget.hpp"
#include "../framework/widgets/leaves/ButtonWidget.hpp"
#include "../framework/text/textSources/StaticText.hpp"

// A simple secondary screen used to demonstrate navigation.
class SecondaryScreen : public Screen
{
public:
    SecondaryScreen();

private:
    StaticText titleText;
    LabelWidget titleLabel;
    StaticText backText;
    LabelWidget backLabel;
    ButtonWidget backButton;
};
