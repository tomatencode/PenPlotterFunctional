#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "../widgets/layouts/VerticalLayout.hpp"
#include "../focusManager/FocusManager.hpp"

constexpr size_t MAX_WIDGETS_PER_SCREEN = 16;

class Router; // forward

class Screen
{
public:
    virtual ~Screen() = default;

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

    // Provide access to the router for navigation from within a screen
    Router* router() const;

protected:
    // Allows derived screens to build their widget tree after construction
    Screen();

    // Initialize the screen root after members are initialized
    void initRoot(Widget* children[], size_t count);
    void initRoot(Widget* rootWidget);

private:
    Widget* root;
    FocusManager focusManager;

    Router* _router = nullptr;
    friend class Router;
    void setRouter(Router* r) { _router = r; }
};

#endif