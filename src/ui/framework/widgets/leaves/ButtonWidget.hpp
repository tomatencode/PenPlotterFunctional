#pragma once

#include <functional>

#include "../core/SelectableWidget.hpp"
#include "../core/Widget.hpp"

// Optional: simple style for button decorations
struct ButtonStyle
{
    Glyph leftNormal  = '[';
    Glyph rightNormal = ']';
    Glyph leftFocused = '>';
    Glyph rightFocused = '<';
    Glyph leftPressed = '-';
    Glyph rightPressed = '-';
    
    ButtonStyle() = default;
    ButtonStyle(Glyph ln, Glyph rn, Glyph lf, Glyph rf, Glyph lp, Glyph rp)
        : leftNormal(ln), rightNormal(rn), leftFocused(lf), rightFocused(rf),
          leftPressed(lp), rightPressed(rp) {}
};

class ButtonWidget : public SelectableWidget
{
public:
    // Construct a button wrapping any child widget.
    // onPress/onRelease are called when the button is pressed/released.
    ButtonWidget(Widget* child,
                 ButtonStyle style = ButtonStyle{},
                 std::function<void()> onPress = nullptr,
                 std::function<void()> onRelease = nullptr
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