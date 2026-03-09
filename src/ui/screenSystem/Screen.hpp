#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "../widgetSystem/Container.hpp"
#include "FocusManager.hpp"

constexpr size_t MAX_WIDGETS_PER_SCREEN = 16;

class Screen
{
public:
    // Constructor: takes an array of Widget* and the number of widgets
    Screen(Widget* children[], size_t count);

    // Render the screen
    virtual void render(Renderer& r);

    // Lifecycle hooks
    virtual void onEnter();
    virtual void onExit();

    // Access selectable widgets
    virtual void handleInput(InputState& input);

private:
    Container root;
    FocusManager focusManager;
};

#endif