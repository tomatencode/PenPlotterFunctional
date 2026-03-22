#include "Screen.hpp"
#include "../widgets/core/CollectSelectables.hpp"
#include "../router/Router.hpp"

namespace ui {

Screen::Screen(std::unique_ptr<widgets::Widget> rootWidget, uint8_t firstFocused)
    : Screen() // build base state then init
{
    _firstFocused = firstFocused;
    initRoot(std::move(rootWidget));
}

Screen::Screen()
    : root(nullptr), focusManager(), _firstFocused(0)
{
}

void Screen::initRoot(std::unique_ptr<widgets::Widget> rootWidget)
{
    root = std::move(rootWidget);

    std::vector<widgets::SelectableWidget*> selectableWidgets;

    // Collect selectable widgets recursively from the root widget
    collectSelectables(root.get(), selectableWidgets);

    focusManager.setWidgets(selectableWidgets, _firstFocused);
}

void Screen::reload() {
    if (root)
        root->reload();

    std::vector<widgets::SelectableWidget*> selectableWidgets;

    // Collect selectable widgets recursively from the root widget
    collectSelectables(root.get(), selectableWidgets);

    focusManager.setWidgets(selectableWidgets, _firstFocused);
}

void Screen::render(Renderer& r)
{
    r.clearBuffer();
    focusManager.refresh();
    root->render(r, {0, 0, static_cast<uint8_t>(LCD_COLS), static_cast<uint8_t>(LCD_ROWS)});
}

void Screen::onEnter() {}
void Screen::onExit() {}

} // namespace ui