#include "Router.hpp"

Router::Router() : _stackCount(0) {}

void Router::pushScreen(Screen* screen)
{
    assert(_stackCount < MAX_STACK);

    if (_stackCount > 0)
        _stack[_stackCount - 1]->onExit();

    _stack[_stackCount++] = screen;
    screen->onEnter();
}

void Router::popScreen()
{
    if (_stackCount == 0) return;

    _stack[_stackCount - 1]->onExit();
    --_stackCount;

    if (_stackCount > 0)
        _stack[_stackCount - 1]->onEnter();
}

void Router::setScreen(Screen* screen)
{
    if (_stackCount > 0)
        _stack[_stackCount - 1]->onExit();

    _stack[0] = screen;
    _stackCount = 1;

    screen->onEnter();
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