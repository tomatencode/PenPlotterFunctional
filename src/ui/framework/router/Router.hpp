#pragma once

#include <vector>
#include <cstddef>
#include <cassert>
#include "../screen/Screen.hpp"
#include "../input/InputState.hpp"

namespace ui {

class Router
{
public:
    Router() {};

    void pushScreen(std::unique_ptr<Screen> screen);
    void popScreen();

    bool canPop() const { return !_stack.empty(); }
    Screen* top() const { return _stack.empty() ? nullptr : _stack.back().get(); }

    // Forward input to the active screen
    void handleInput(InputState& input);

    // Forward rendering to the active screen
    void render(Renderer& r);
private:
    std::vector<std::unique_ptr<Screen>> _stack;
};

} // namespace ui