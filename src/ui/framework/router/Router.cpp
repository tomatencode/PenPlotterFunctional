#include "Router.hpp"

namespace ui {


void Router::pushScreen(Screen* screen)
{
    if (screen == nullptr) return;

    _stack.push_back(screen);
    screen->setRouter(this);
    screen->onEnter();
}

void Router::popScreen()
{
    if (!canPop()) return;

    top()->onExit();
    top()->setRouter(nullptr);
    _stack.pop_back();

    if (canPop())
    {
        top()->setRouter(this);
        top()->onEnter();
    }
}

void Router::handleInput(InputState& input)
{
    if (_stack.empty()) return;
    _stack.back()->handleInput(input);
}

void Router::render(Renderer& r)
{
    if (_stack.empty()) return;
    _stack.back()->render(r);
}

} // namespace ui