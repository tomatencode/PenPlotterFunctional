#pragma once

#include <functional>
#include <memory>
#include <concepts>

#include "ui/framework/widgets/core/Container.hpp"
#include "ui/framework/widgets/leaves/Label.hpp"
#include "ui/components/WifiIndicator.hpp"
#include "ui/framework/widgets/leaves/Button.hpp"
#include "ui/framework/widgets/leaves/Spacer.hpp"
#include "ui/framework/widgets/leaves/Switch.hpp"
#include "ui/framework/widgets/layouts/LinearLayout.hpp"
#include "ui/framework/text/GlyphStringProvider.hpp"

namespace ui {
namespace components {

struct HeaderLineProps {
    GlyphStringProvider textProvider;
    std::function<bool()> wifiStatusProvider;
    std::function<void()> onBackPress = nullptr;
};

class HeaderLine: public ui::widgets::Container
{
public:
    HeaderLine(HeaderLineProps props)
    : Container(
        std::make_unique<widgets::LinearLayout>(
            widgets::LinearLayoutStyle{.axis = widgets::Axis::Horizontal, .spacingMode = widgets::SpacingMode::SpaceBetween},
            std::make_unique<components::WifiIndicator>(WifiIndicatorProps{.wifiStatusProvider = props.wifiStatusProvider}),
            std::make_unique<widgets::Label>(props.textProvider),
            std::make_unique<widgets::Switch<bool>>(
                widgets::SwitchProps{
                    .selector = [onBackPress = props.onBackPress]() { return onBackPress != nullptr; },
                    .evaluationMode = widgets::SwitchEvaluationMode::Lazy
                },
                std::make_unique<widgets::Switch<bool>::Branch>(
                    true,
                    std::make_unique<widgets::Button>(
                        widgets::ButtonProps{.onPress = props.onBackPress},
                        std::make_unique<widgets::Label>("Back")
                    )
                ),
                std::make_unique<widgets::Switch<bool>::Branch>(
                    false,
                    std::make_unique<widgets::Spacer>(1, 1)
                )
            )
        )
    )
    {}
};

} // namespace components
} // namespace ui