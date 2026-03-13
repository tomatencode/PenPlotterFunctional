#include "Screen.hpp"
#include "../widgets/core/WidgetUtils.hpp"

Screen::Screen(Widget* children[], size_t count)
    : root(new VerticalLayout(children, count)), focusManager()
{
    SelectableWidget* selectableWidgets[MAX_WIDGETS_PER_SCREEN] = { nullptr };
    size_t selectableCount = 0;

    // Collect selectable widgets recursively from the root layout
    collectSelectables(root, selectableWidgets, selectableCount);
    focusManager.setWidgets(selectableWidgets, selectableCount);
}

Screen::Screen(Widget* rootWidget)
    : root(rootWidget), focusManager()
{
    SelectableWidget* selectableWidgets[MAX_WIDGETS_PER_SCREEN] = { nullptr };
    size_t selectableCount = 0;

    // Collect selectable widgets recursively from the root widget
    collectSelectables(root, selectableWidgets, selectableCount);
    focusManager.setWidgets(selectableWidgets, selectableCount);
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