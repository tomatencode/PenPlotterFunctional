#include "UI.hpp"
#include <string>

UI::UI(TextDisplay& display) : _display(display) {}

void UI::update()
{
    _display.clear();
    _display.drawText(0, 0, "Pen Plotter");
}