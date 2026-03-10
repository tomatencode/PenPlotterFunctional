#ifndef BUTTON_WIDGET_HPP
#define BUTTON_WIDGET_HPP

#include "../widgetSystem/SelectableWidget.hpp"
#include "../widgetSystem/Widget.hpp"
#include <functional>

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
                 std::function<void()> onPress = nullptr,
                 std::function<void()> onRelease = nullptr,
                 Alignment align = {HorizontalAlignment::Left, VerticalAlignment::Top}
                );
    
    Size measure() const override;
    void render(Renderer& r, Rect canvasBox) override;
    void handleInput(InputState& input) override;

private:
    Widget* _child;               // the child widget to render inside the button
    ButtonStyle _style;           // visual decorations
    std::function<void()> _onPress;
    std::function<void()> _onRelease;

    bool _isPressed = false;

    void onFocusGained() override;
    void onFocusLost() override;
};

#endif