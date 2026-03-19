#include "Router.hpp"

namespace ui {

Router::Router() : _stackCount(0) {}

bool Router::isEmpty() const
{
    return _stackCount == 0;
}

bool Router::canPush() const
{
    return _stackCount < MAX_STACK;
}

bool Router::canPop() const
{
    return _stackCount > 0;
}

size_t Router::stackSize() const
{
    return _stackCount;
}

Screen* Router::top() const
{
    return _stackCount ? _stack[_stackCount - 1] : nullptr;
}

void Router::pushScreen(Screen* screen)
{
    if (screen == nullptr) return;
    if (!canPush()) return; // stack is full

    if (!isEmpty())
        top()->onExit();

    _stack[_stackCount++] = screen;
    screen->setRouter(this);
    screen->onEnter();
}

void Router::popScreen()
{
    if (!canPop()) return;

    top()->onExit();
    top()->setRouter(nullptr);
    --_stackCount;

    if (canPop())
    {
        top()->setRouter(this);
        top()->onEnter();
    }
}

void Router::handleInput(InputState& input)
{
    if (_stackCount == 0) return;

    // Forward input to top screen
    _stack[_stackCount - 1]->handleInput(input);
}

void Router::render(Renderer& r)
{
    if (_stackCount == 0) return;

    _stack[_stackCount - 1]->render(r);
}

} // namespace ui