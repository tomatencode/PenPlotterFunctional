#include "Screen.hpp"
#include "../widgets/core/WidgetUtils.hpp"
#include "../router/Router.hpp"

Screen::Screen(Widget* children[], size_t count)
    : Screen() // build base state then init
{
    initRoot(children, count);
}

Screen::Screen(Widget* rootWidget)
    : Screen() // build base state then init
{
    initRoot(rootWidget);
}

Screen::Screen()
    : root(nullptr), focusManager()
{
}

void Screen::initRoot(Widget* children[], size_t count)
{
    root = new VerticalLayout(children, count);

    SelectableWidget* selectableWidgets[MAX_WIDGETS_PER_SCREEN] = { nullptr };
    size_t selectableCount = 0;

    // Collect selectable widgets recursively from the root layout
    collectSelectables(root, selectableWidgets, selectableCount);
    focusManager.setWidgets(selectableWidgets, selectableCount);
}

void Screen::initRoot(Widget* rootWidget)
{
    root = rootWidget;

    SelectableWidget* selectableWidgets[MAX_WIDGETS_PER_SCREEN] = { nullptr };
    size_t selectableCount = 0;

    // Collect selectable widgets recursively from the root widget
    collectSelectables(root, selectableWidgets, selectableCount);
    focusManager.setWidgets(selectableWidgets, selectableCount);
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