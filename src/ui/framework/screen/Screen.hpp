#pragma once

#include <memory>

#include "../widgets/layouts/LinearLayout.hpp"
#include "../focusManager/FocusManager.hpp"

namespace ui {

class Router; // forward declaration

class Screen
{
public:
    explicit Screen(std::unique_ptr<widgets::Widget> rootWidget, uint8_t firstFocused = 0);
    virtual ~Screen() = default;

    void setRouter(Router* r) { _router = r; }

    // Render the screen
    virtual void render(Renderer& r);
    virtual void update() {};

    // Reload all widgets, useful for dynamic content updates
    virtual void reload();

    // Lifecycle hooks
    virtual void onEnter() {};
    virtual void onExit() {};
    virtual void onPause() {};
    virtual void onUnPause() {};

    
    virtual void handleInput(InputState& input) {
        _focusManager.handleInput(input);
    }

protected:
    // Allows derived screens to build their widget tree after construction
    Screen();

    // Provide access to the router for navigation from within a screen
    Router* router() const { return _router; }

    void setRoot(std::unique_ptr<widgets::Widget> rootWidget, uint8_t firstFocused = 0);

private:
    std::unique_ptr<widgets::Widget> _root;
    FocusManager _focusManager;

    Router* _router = nullptr;
};

} // namespace ui