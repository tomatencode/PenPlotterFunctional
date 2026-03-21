#pragma once

#include <functional>
#include <memory>

#include "../core/SelectableWidget.hpp"
#include "../core/Widget.hpp"

namespace ui {
namespace widgets {

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
    // Takes ownership of the child widget.
    // The child should be heap-allocated and not managed elsewhere.
    ButtonWidget(std::unique_ptr<Widget> child,
                 ButtonStyle style = ButtonStyle{},
                 std::function<void()> onPress = nullptr,
                 std::function<void()> onRelease = nullptr
                );

    Size measure() const override;
    void render(Renderer& r, Rect canvasBox) override;
    void handleInput(InputState& input) override;

private:
    Widget* _child;               // the child widget to render inside the button
    std::unique_ptr<Widget> _ownedChild;
    ButtonStyle _style;           // visual decorations
    std::function<void()> _onPress;
    std::function<void()> _onRelease;

    bool _isPressed = false;

    void onFocusGained() override;
    void onFocusLost() override;
};

} // namespace widgets
} // namespace ui
