#pragma once

#include <functional>
#include <memory>

#include "../core/ISelectable.hpp"
#include "../core/Container.hpp"
#include "Label.hpp"

namespace ui {
namespace widgets {

struct ButtonStyle
{
    Glyph leftNormal   = '[';
    Glyph rightNormal  = ']';
    Glyph leftFocused  = '>';
    Glyph rightFocused = '<';
    Glyph leftPressed  = '-';
    Glyph rightPressed = '-';
};

struct ButtonProps
{
    ButtonStyle style          = {};
    std::function<void()> onPress   = nullptr;
    std::function<void()> onRelease = nullptr;
};

class Button : public Container, public ISelectable
{
public:

    Button(ButtonProps props = {},
           std::unique_ptr<Widget> child = nullptr)
        : Container(std::move(child)),
          _props(std::move(props))
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
    ButtonProps _props;

    bool _isPressed = false;

    void onFocusGained() override;
    void onFocusLost() override;
};

} // namespace widgets
} // namespace ui
