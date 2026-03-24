#pragma once

#include <functional>
#include <memory>

#include "../framework/widgets/leaves/Button.hpp"

namespace ui {
namespace components {

struct PressHoldButtonStyle {
    Glyph leftNormal = '[';
    Glyph rightNormal = ']';
    Glyph leftFocused = '>';
    Glyph rightFocused = '<';
    Glyph leftPressed = '-';
    Glyph rightPressed = '-';
    Glyph HoldCountdownFilled = '=';
    Glyph HoldCountdownEmpty = ' ';
};

class PressHoldButton: public ui::widgets::Button
{
public:
    template <typename TextType>
    PressHoldButton(TextType text, PressHoldButtonStyle style, std::function<void()> onHoldRelease, uint16_t holdTimeMs = 2000)
        : Button(text, widgets::ButtonStyle{
            .leftNormal = style.leftNormal,
            .rightNormal = style.rightNormal,
            .leftFocused = style.leftFocused,
            .rightFocused = style.rightFocused,
            .leftPressed = style.leftPressed,
            .rightPressed = style.rightPressed
        }, nullptr, nullptr)
        , _onHoldRelease(std::move(onHoldRelease))
        , _holdTimeMs(holdTimeMs)
        , _style(std::move(style))
    {}

private:
    std::function<void()> _onHoldRelease;
    uint16_t _holdTimeMs;
    uint32_t _pressStartTime = 0;

    PressHoldButtonStyle _style;

    void handleInput(InputState& input) override;
    void render(Renderer& r, widgets::Rect canvasBox) override;
};

} // namespace components
} // namespace ui