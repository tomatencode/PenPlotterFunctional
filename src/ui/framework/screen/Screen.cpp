#include "Screen.hpp"
#include "../widgets/core/CollectSelectables.hpp"
#include "../router/Router.hpp"

namespace ui {

Screen::Screen(std::unique_ptr<widgets::Widget> rootWidget)
    : Screen() // build base state then init
{
    initRoot(std::move(rootWidget));
}

Screen::Screen()
    : root(nullptr), focusManager()
{
}

void Screen::initRoot(std::unique_ptr<widgets::Widget> rootWidget)
{
    root = std::move(rootWidget);

    std::vector<widgets::SelectableWidget*> selectableWidgets;

    // Collect selectable widgets recursively from the root widget
    collectSelectables(root.get(), selectableWidgets);

    focusManager.setWidgets(selectableWidgets);
}

Router* Screen::router() const
{
    return _router;
}

void Screen::render(Renderer& r)
{
    r.clearBuffer();
    root->render(r, {0, 0, static_cast<uint8_t>(LCD_COLS), static_cast<uint8_t>(LCD_ROWS)});
}

void Screen::handleInput(InputState& input)
{
    focusManager.handleInput(input);
}

void Screen::onEnter() {}
void Screen::onExit() {}

} // namespace ui