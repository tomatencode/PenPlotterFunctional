#pragma once

#include <functional>
#include <memory>

#include "../core/ISelectable.hpp"
#include "../core/Container.hpp"
#include "Label.hpp"

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

class Button : public Container, public ISelectable
{
public:

    Button(ButtonStyle style = ButtonStyle{},
           std::function<void()> onPress = nullptr,
           std::function<void()> onRelease = nullptr,
           std::unique_ptr<Widget> child = nullptr
          )
        : Container(std::move(child)),
          _style(style),
          _onPress(std::move(onPress)),
          _onRelease(std::move(onRelease))
    {}

    Size measure() const override;

    void render(Renderer& r, Rect canvasBox) override;
    
    void handleInput(InputState& input) override;

    ISelectable* tryGetSelectable() override { return this; }

    bool isPressed() const { return _isPressed; }

    void reload() override {
        ISelectable::reload();
        Container::reload();
        _isPressed = false;
    }

private:
    ButtonStyle _style;           // visual decorations
    std::function<void()> _onPress;
    std::function<void()> _onRelease;

    bool _isPressed = false;

    void onFocusGained() override;
    void onFocusLost() override;
};

} // namespace widgets
} // namespace ui
