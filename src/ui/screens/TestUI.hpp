#pragma once

#include "../framework/screen/Screen.hpp"
#include "../framework/widgets/leaves/LabelWidget.hpp"
#include "../framework/widgets/leaves/ButtonWidget.hpp"

// Forward declare secondary screen to avoid including it here.
class SecondaryScreen;

// A simple screen that demonstrates widget layout, focus, and button interaction.
class TestScreen : public Screen
{
public:
    TestScreen();

private:
    // Non-owning pointers to widgets owned by the root layout
    LabelWidget* titleLabel = nullptr;
    LabelWidget* btnLabel = nullptr;
    ButtonWidget* button = nullptr;
};

// Convenience helper for getting a screen instance (optional).
Screen* createTestScreen();
