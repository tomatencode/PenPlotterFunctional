#pragma once

#include <optional>

#include "config/hardwareConfig.hpp"

#include "ui/framework/screen/Screen.hpp"
#include "../ScreensContext.hpp"

// Include related screens to enable navigation
#include "ui/framework/router/Router.hpp"

// Include components and widgets used in this screen
#include "ui/components/HeaderLine.hpp"
#include "ui/framework/widgets/leaves/Button.hpp"
#include "ui/framework/widgets/leaves/Label.hpp"
#include "ui/framework/widgets/leaves/ValueSelector.hpp"
#include "ui/framework/widgets/leaves/Switch.hpp"
#include "ui/framework/widgets/layouts/LinearLayout.hpp"

namespace ui {
namespace screens {

class ManualControllScreen : public Screen
{
public:
    ManualControllScreen(const ScreensContext& ctx)
    : Screen(
        std::make_unique<widgets::LinearLayout>(
            widgets::LinearLayoutStyle{.axis = widgets::Axis::Vertical, .horizontalAlign = widgets::HorizontalAlignment::Center},
            std::make_unique<components::HeaderLine>(components::HeaderLineProps{
                .textProvider = "Manual",
                .wifiStatusProvider = ctx.wifiStatusProvider,
                .onBackPress = [this]() {
                    if (router()) router()->popScreen();
                }
            }),

            std::make_unique<widgets::Switch<bool>>(
                widgets::SwitchProps{
                    .selector = [this]() { return _gcodeToken.has_value(); },
                    .evaluationMode = widgets::SwitchEvaluationMode::Eager
                },
                std::make_unique<widgets::Switch<bool>::Branch>(
                    true,
                    std::make_unique<widgets::LinearLayout>(
                        widgets::LinearLayoutStyle{
                            .axis = widgets::Axis::Vertical,
                            .horizontalAlign = widgets::HorizontalAlignment::Center
                        },

                        std::make_unique<widgets::Button>(
                            widgets::ButtonProps{
                                .onPress = [this]() {
                                    _gcodeToken->send("G28"); // Home all axes
                                    _currentTargetX = 0;
                                    _currentTargetY = 0;
                                    _isHomed = true;
                                }
                            },
                            std::make_unique<widgets::Label>("Do homing")
                        ),

                        std::make_unique<widgets::LinearLayout>(
                            widgets::LinearLayoutStyle{
                                .axis = widgets::Axis::Horizontal,
                                .spacingMode = widgets::SpacingMode::SpaceAround,
                            },

                            std::make_unique<widgets::ValueSelector<int>>(
                                widgets::ValueSelectorProps<int>{
                                    .getValue = [ctx]() {
                                        return ctx.motionState.getMachineX();
                                    },
                                    .next = [this](int current) {
                                        _currentTargetX += 5;
                                        if (_isHomed && _currentTargetX > MAX_X_MM) {
                                            _currentTargetX = (MAX_X_MM / 5) * 5; // Round down to nearest multiple of 5
                                        }
                                        _lastTagetChangeMS = millis();
                                    },
                                    .prev = [this](int current) {
                                        _currentTargetX -= 5;
                                        if (_isHomed && _currentTargetX < 0) {
                                            _currentTargetX = 0;
                                        }
                                        _lastTagetChangeMS = millis();
                                    },
                                    .toString = [this](int value) {
                                        if (_currentTargetX == value) {
                                            return "X: " + std::to_string(value);
                                        } else {
                                            return "X: " + std::to_string(value) + "->" + std::to_string(_currentTargetX);
                                        }
                                    }
                                }
                            ),

                            std::make_unique<widgets::ValueSelector<int>>(
                                widgets::ValueSelectorProps<int>{
                                    .getValue = [ctx]() {
                                        return ctx.motionState.getMachineY();
                                    },
                                    .next = [this](int current) {
                                        _currentTargetY += 5;
                                        if (_isHomed && _currentTargetY > MAX_Y_MM) {
                                            _currentTargetY = (MAX_Y_MM / 5) * 5; // Round down to nearest multiple of 5
                                        }
                                        _lastTagetChangeMS = millis();
                                    },
                                    .prev = [this](int current) {
                                        _currentTargetY -= 5;
                                        if (_isHomed && _currentTargetY < 0) {
                                            _currentTargetY = 0;
                                        }
                                        _lastTagetChangeMS = millis();
                                    },
                                    .toString = [this](int value) {
                                        if (_currentTargetY == value) {
                                            return "Y: " + std::to_string(value);
                                        } else {
                                            return "Y: " + std::to_string(value) + "->" + std::to_string(_currentTargetY);
                                        }
                                    }
                                }
                            )
                        ),

                        std::make_unique<widgets::Button>(
                            widgets::ButtonProps{
                                .onPress = [this, ctx]() {
                                    if (ctx.motionState.penIsDown()) {
                                        _gcodeToken->send("M5"); // Pen up
                                    } else {
                                        _gcodeToken->send("M3"); // Pen down
                                    }
                                }
                            },
                            std::make_unique<widgets::Label>([ctx](){
                                if (ctx.motionState.penIsDown()) {
                                    return "Pen up";
                                } else {
                                    return "Pen down";
                                }
                            })
                        )
                    )
                ),

                std::make_unique<widgets::Switch<bool>::Branch>(
                    false,
                    std::make_unique<widgets::LinearLayout>(
                        widgets::LinearLayoutStyle{
                            .axis = widgets::Axis::Vertical,
                            .horizontalAlign = widgets::HorizontalAlignment::Center
                        },

                        std::make_unique<widgets::Label>("GCodeSender token"),
                        std::make_unique<widgets::Label>("not available!")
                    )
                )
            )
        )
    , 0), _motionState(ctx.motionState), _gcodeSender(ctx.gcodeSender)
    {
        auto tokenOpt = _gcodeSender.tryAcquire();
        if (tokenOpt) {
            _gcodeToken = std::move(tokenOpt);
            _gcodeToken->send("G90"); // Ensure we're in absolute mode
            _currentTargetX = _motionState.getMachineX();
            _currentTargetY = _motionState.getMachineY();
        } else {
            ESP_LOGI("ManualControllScreen", "Failed to acquire GCodeSender token");
        }
    }

    void update() override {
        // If we have the token, we can update our current position from the motion state
        if (_gcodeToken) {
            if ((_currentTargetX != _motionState.getMachineX() || _currentTargetY != _motionState.getMachineY())
                && (_quedTargetX != _currentTargetX || _quedTargetY != _currentTargetY)
                && (millis() - _lastTagetChangeMS > 250))
            {
                _gcodeToken->send("G0 X" + std::to_string(_currentTargetX) + " Y" + std::to_string(_currentTargetY));
                _quedTargetX = _currentTargetX;
                _quedTargetY = _currentTargetY;
            }
        } else {
            // Try to acquire the token if we don't have it yet
            auto tokenOpt = _gcodeSender.tryAcquire();
            if (tokenOpt) {
                _gcodeToken = std::move(tokenOpt);
                _gcodeToken->send("G90"); // Ensure we're in absolute mode
                _currentTargetX = _motionState.getMachineX();
                _currentTargetY = _motionState.getMachineY();
            }
        }
    }
    
private:
    std::optional<GCodeSender::Token> _gcodeToken;
    MotionState& _motionState;
    GCodeSender& _gcodeSender;

    bool _isHomed = false;

    int _currentTargetX = 0;
    int _currentTargetY = 0;

    int _quedTargetX = 0;
    int _quedTargetY = 0;

    uint32_t _lastTagetChangeMS = 0;
};

} // namespace screens
} // namespace ui