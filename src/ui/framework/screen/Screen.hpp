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
    explicit Screen(std::unique_ptr<widgets::Widget> rootWidget);

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
    void initRoot(std::unique_ptr<widgets::Widget> rootWidget);

private:
    std::unique_ptr<widgets::Widget> root;
    FocusManager focusManager;

    Router* _router = nullptr;
    friend class Router;
    void setRouter(Router* r) { _router = r; }
};

} // namespace ui