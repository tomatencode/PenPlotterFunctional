#pragma once

#include "../framework/screen/Screen.hpp"

// Forward declare secondary screen to avoid including it here.
class SecondaryScreen;

// A simple screen that demonstrates widget layout, focus, and button interaction.
class TestScreen : public ui::Screen
{
public:
    TestScreen();
};

// Convenience helper for getting a screen instance (optional).
ui::Screen* createTestScreen();
