#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "../widgets/layouts/VerticalLayout.hpp"
#include "../focusManager/FocusManager.hpp"

constexpr size_t MAX_WIDGETS_PER_SCREEN = 16;

class Screen
{
public:
    // Constructor: takes an array of Widget* and auto-stacks them vertically
    Screen(Widget* children[], size_t count);
    
    // Constructor: takes a single root widget
    Screen(Widget* rootWidget);

    // Render the screen
    virtual void render(Renderer& r);

    // Lifecycle hooks
    virtual void onEnter();
    virtual void onExit();

    // Access selectable widgets
    virtual void handleInput(InputState& input);

private:
    Widget* root;
    FocusManager focusManager;
};

#endif