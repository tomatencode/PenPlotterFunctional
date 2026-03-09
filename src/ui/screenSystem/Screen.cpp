#include "Screen.hpp"

Screen::Screen(Widget* children[], size_t count)
    : root({0, 0, LCD_COLS, LCD_ROWS}, children, count), focusManager()
{
    SelectableWidget* selectableWidgets[MAX_WIDGETS_PER_SCREEN] = { nullptr };
    size_t selectableCount = 0;

    // Collect selectable widgets recursively from the root container
    root.collectSelectables(selectableWidgets, selectableCount);
    focusManager.setWidgets(selectableWidgets, selectableCount);
}

void Screen::render(Renderer& r)
{
    r.clearBuffer();
    root.render(r, {0, 0, LCD_COLS, LCD_ROWS});
}

void Screen::handleInput(InputState& input)
{
    focusManager.handleInput(input);
}

void Screen::onEnter() {}
void Screen::onExit() {}