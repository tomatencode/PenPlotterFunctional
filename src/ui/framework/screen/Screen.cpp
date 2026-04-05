#include "Screen.hpp"
#include "../router/Router.hpp"

namespace ui {

Screen::Screen(std::unique_ptr<widgets::Widget> rootWidget, uint8_t firstFocused)
    : _root(std::move(rootWidget)),
      _focusManager(_root.get(), firstFocused)
{}

Screen::Screen()
    : _root(nullptr), _focusManager(nullptr)
{}

void Screen::setRoot(std::unique_ptr<widgets::Widget> rootWidget, uint8_t firstFocused) {
    _root = std::move(rootWidget);
    _focusManager.setRoot(_root.get(), firstFocused);
}

void Screen::reload() {
    if (_root)
        _root->reload();
    
    _focusManager.refresh();
}

void Screen::render(Renderer& r)
{
    r.clearBuffer();
    if (_root)
        _root->render(r, {0, 0, LCD_COLS, LCD_ROWS});
}

} // namespace ui