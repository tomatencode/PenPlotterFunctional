#pragma once

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
    // onPress/onRelease receive an optional context pointer.
    ButtonWidget(Widget* child,
                 ButtonStyle style = ButtonStyle{},
                 void (*onPress)(void* context) = nullptr,
                 void (*onRelease)(void* context) = nullptr,
                 void* context = nullptr
                );


    Size measure() const override;
    void render(Renderer& r, Rect canvasBox) override;
    void handleInput(InputState& input) override;

private:
    Widget* _child;               // the child widget to render inside the button
    ButtonStyle _style;           // visual decorations
    void (*_onPress)(void*);
    void (*_onRelease)(void*);
    void* _context;

    bool _isPressed = false;

    void onFocusGained() override;
    void onFocusLost() override;
};