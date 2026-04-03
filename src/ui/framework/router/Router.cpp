#include "Router.hpp"

namespace ui {


void Router::pushScreen(std::unique_ptr<Screen> screen)
{
    if (screen == nullptr) return;

    if (top()) {
        top()->onExit();
        top()->setRouter(nullptr);
    }
    _stack.push_back(std::move(screen));
    top()->setRouter(this);
    top()->onEnter();
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
    if (!top()) return;
    top()->handleInput(input);
}

void Router::render(Renderer& r)
{
    if (!top()) return;
    top()->render(r);
}

} // namespace ui