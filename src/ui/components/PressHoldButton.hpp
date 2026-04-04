#pragma once

#include <functional>
#include <memory>

#include "../framework/widgets/leaves/Button.hpp"
#include "../framework/widgets/leaves/Label.hpp"

namespace ui {
namespace components {

struct PressHoldButtonStyle {
    GlyphStringProvider leftNormal = "[";
    GlyphStringProvider rightNormal = "]";
    GlyphStringProvider leftFocused = ">";
    GlyphStringProvider rightFocused = "<";
    GlyphStringProvider leftPressed = "-";
    GlyphStringProvider rightPressed = "-";
    GlyphString HoldCountdownFilled = "=";
    GlyphString HoldCountdownEmpty = " ";
    GlyphString HoldReleaseAnimation = "-";
};

class PressHoldButton: public ui::widgets::Button
{
public:
    template <typename TextType>
    PressHoldButton(TextType text, PressHoldButtonStyle style, std::function<void()> onHoldRelease, uint16_t holdTimeMs = 2000, uint16_t relAnimationTimeMs = 500)
        : Button(
            widgets::ButtonProps{
                .style = widgets::ButtonStyle{
                    .leftNormal   = style.leftNormal,
                    .rightNormal  = style.rightNormal,
                    .leftFocused  = style.leftFocused,
                    .rightFocused = style.rightFocused,
                    .leftPressed  = style.leftPressed,
                    .rightPressed = style.rightPressed
                }
            },
            std::make_unique<widgets::Label>(text)
          ),
          _onHoldRelease(std::move(onHoldRelease)),
          _holdTimeMs(holdTimeMs),
          _relAnimationTimeMs(relAnimationTimeMs),
          _style(std::move(style))
    {}

private:
    std::function<void()> _onHoldRelease;
    uint16_t _holdTimeMs;
    uint16_t _relAnimationTimeMs;
    uint32_t _lastPressStartTime = 0;
    uint32_t _lastReleaseStartTime = 0;

    bool _inRelAnimation = false;
    bool _callbackPending = false;

    PressHoldButtonStyle _style;

    void handleInput(InputState& input) override;
    void render(Renderer& r, widgets::Box canvasBox) override;
};

} // namespace components
} // namespace ui