#ifndef BUTTON_WIDGET_HPP
#define BUTTON_WIDGET_HPP

#include "../widgetSystem/SelectableWidget.hpp"

class ButtonWidget : public SelectableWidget
{
public:
    ButtonWidget(Rect box, const char* label, void (*onPress)() = nullptr, void (*onRelease)() = nullptr);

    void render(Renderer& r, Rect canvasBox) override;
    void handleInput(InputState& input) override;

private:
    const char* label;
    void (*onPress)();
    void (*onRelease)();

    bool _isPressed = false;

    void onFocusGained() override;
    void onFocusLost() override;
};

#endif