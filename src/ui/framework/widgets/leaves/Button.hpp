#pragma once

#include <functional>
#include <memory>

#include "../core/Selectable.hpp"
#include "../core/Widget.hpp"
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

class Button : public Selectable
{
public:
    // creates a Label
    template <typename TextType>
    Button(TextType text,
        ButtonStyle style = ButtonStyle{},
        std::function<void()> onPress = nullptr,
        std::function<void()> onRelease = nullptr
        )
        : _label(std::make_unique<Label>(text)),
        _style(style),
        _onPress(std::move(onPress)),
        _onRelease(std::move(onRelease))
    {}

    Size measure() const override;

    void render(Renderer& r, Rect canvasBox) override;
    
    void handleInput(InputState& input) override;

    bool isPressed() const { return _isPressed; }

    void reload() override {
        Selectable::reload();
        if (_label)
            _label->reload();
        _isPressed = false;
    }

private:
    std::unique_ptr<Label> _label;
    ButtonStyle _style;           // visual decorations
    std::function<void()> _onPress;
    std::function<void()> _onRelease;

    bool _isPressed = false;

    void onFocusGained() override;
    void onFocusLost() override;
};

} // namespace widgets
} // namespace ui
