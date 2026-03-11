#ifndef BUTTON_WIDGET_HPP
#define BUTTON_WIDGET_HPP

#include "../widgetSystem/SelectableWidget.hpp"
#include "../widgetSystem/Widget.hpp"

// Optional: simple style for button decorations
struct ButtonStyle
{
    Glyph leftNormal  = '[';
    Glyph rightNormal = ']';
    Glyph leftFocused = '>';
    Glyph rightFocused = '<';
    Glyph leftPressed = '-';
    Glyph rightPressed = '-';
};

class ButtonWidget : public SelectableWidget
{
public:
    // Construct a button wrapping any child widget
    ButtonWidget(Rect box,
                 Widget* child,
                 ButtonStyle style = ButtonStyle{},
                 void (*onPress)() = nullptr,
                 void (*onRelease)() = nullptr,
                 Alignment align = {HorizontalAlignment::Left, VerticalAlignment::Top}
                );
    
    Size measure() const override;
    void render(Renderer& r, Rect canvasBox) override;
    void handleInput(InputState& input) override;

private:
    Widget* _child;               // the child widget to render inside the button
    ButtonStyle _style;           // visual decorations
    void (*_onPress)();
    void (*_onRelease)();

    bool _isPressed = false;

    void onFocusGained() override;
    void onFocusLost() override;
};

#endif