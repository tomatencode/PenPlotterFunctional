#pragma once

#include <functional>
#include <memory>

#include "ui/framework/widgets/leaves/Button.hpp"
#include "ui/framework/widgets/leaves/Label.hpp"

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

struct PressHoldButtonProps {
    PressHoldButtonStyle style = {};
    std::function<void()> onHoldComplete = nullptr;
    uint16_t holdDurationMs = 2000;
    uint16_t relAnimationTimeMs = 500;
};

class PressHoldButton: public ui::widgets::Button
{
public:
    PressHoldButton(PressHoldButtonProps props, std::unique_ptr<Widget> child = nullptr)
        : Button(
            widgets::ButtonProps{
                .style = widgets::ButtonStyle{
                    .leftNormal   = props.style.leftNormal,
                    .rightNormal  = props.style.rightNormal,
                    .leftFocused  = props.style.leftFocused,
                    .rightFocused = props.style.rightFocused,
                    .leftPressed  = props.style.leftPressed,
                    .rightPressed = props.style.rightPressed
                }
            },
            std::move(child)
          ),
          _onHoldRelease(std::move(props.onHoldComplete)),
          _holdTimeMs(props.holdDurationMs),
          _relAnimationTimeMs(props.relAnimationTimeMs),
          _style(std::move(props.style))
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