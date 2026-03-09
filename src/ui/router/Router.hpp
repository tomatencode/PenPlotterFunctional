#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <array>
#include <cstddef>
#include <cassert>
#include "../screenSystem/Screen.hpp"
#include "../InputState.hpp"

class Router
{
public:
    static constexpr size_t MAX_STACK = 8; // adjust for embedded memory limits

    Router();

    // Push a new screen onto the stack (current screen stays below)
    void pushScreen(Screen* screen);

    // Pop the current screen from the stack
    void popScreen();

    // Replace the current screen (clears stack)
    void setScreen(Screen* screen);

    // Forward input to the active screen
    void handleInput(InputState& input);

    // Forward rendering to the active screen
    void render(Renderer& r);
private:
    std::array<Screen*, MAX_STACK> _stack;
    size_t _stackCount;
};

#endif