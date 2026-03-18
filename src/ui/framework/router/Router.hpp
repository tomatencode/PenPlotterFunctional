#pragma once

#include <array>
#include <cstddef>
#include <cassert>
#include "../screen/Screen.hpp"
#include "../input/InputState.hpp"

namespace ui {

class Router
{
public:
    static constexpr size_t MAX_STACK = 8; // adjust for embedded memory limits

    Router();

    // Push a new screen onto the stack (current screen stays below)
    // Returns false if the stack is full or screen is null.
    void pushScreen(Screen* screen);

    // Pop the current screen from the stack
    // Returns false if the stack is already empty.
    void popScreen();

    // Replace the current screen (clears stack)
    // Returns false if screen is null.
    void setScreen(Screen* screen);

    // Query the stack for safe usage
    bool isEmpty() const;
    bool canPush() const;
    bool canPop() const;
    size_t stackSize() const;
    Screen* top() const;

    // Forward input to the active screen
    void handleInput(InputState& input);

    // Forward rendering to the active screen
    void render(Renderer& r);
private:
    std::array<Screen*, MAX_STACK> _stack;
    size_t _stackCount;
};

} // namespace ui