#pragma once

#include <memory>

#include "../widgets/layouts/VerticalLayout.hpp"
#include "../focusManager/FocusManager.hpp"

namespace ui {

constexpr size_t MAX_WIDGETS_PER_SCREEN = 16;

class Router; // forward

class Screen
{
public:
    virtual ~Screen() = default;

    // Constructor: takes ownership of a single root widget
    explicit Screen(std::unique_ptr<widgets::Widget> rootWidget, uint8_t firstFocused = 0);

    // Render the screen
    virtual void render(Renderer& r);

    virtual void reload();

    // Lifecycle hooks
    virtual void onEnter();
    virtual void onExit();

    // Access selectable widgets
    virtual void handleInput(InputState& input) {
        focusManager.handleInput(input);
    }

protected:
    // Allows derived screens to build their widget tree after construction
    Screen();

    // Provide access to the router for navigation from within a screen
    Router* router() const { return _router; }

    // Initialize the screen root after members are initialized
    void initRoot(std::unique_ptr<widgets::Widget> rootWidget);

private:
    std::unique_ptr<widgets::Widget> root;
    FocusManager focusManager;

    uint8_t _firstFocused = 0; // Store first focused index for reloads

    Router* _router = nullptr;
    friend class Router;
    void setRouter(Router* r) { _router = r; }
};

} // namespace ui