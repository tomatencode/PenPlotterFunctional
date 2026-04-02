#pragma once

#include <functional>
#include <memory>
#include <concepts>

#include "../framework/widgets/core/Container.hpp"
#include "../framework/widgets/leaves/Label.hpp"
#include "../components/WifiIndicator.hpp"
#include "../framework/widgets/leaves/Button.hpp"
#include "../framework/widgets/leaves/Spacer.hpp"
#include "../framework/widgets/leaves/Switch.hpp"
#include "../framework/widgets/layouts/LinearLayout.hpp"

namespace ui {
namespace components {

class HeaderLine: public ui::widgets::Container
{
public:
    template <typename TextType>
    requires std::constructible_from<widgets::Label, TextType>
    HeaderLine(TextType textProvider,
               std::function<bool()> WifiStatusProvider,
               std::function<void()> onBackPress = nullptr
              )
    : Container(
        std::make_unique<widgets::LinearLayout>(
            widgets::LinearLayoutStyle{.axis = widgets::Axis::Horizontal, .spacingMode = widgets::SpacingMode::SpaceBetween},
            std::make_unique<components::WifiIndicator>(WifiStatusProvider),
            std::make_unique<widgets::Label>(textProvider),
            std::make_unique<widgets::Switch<bool>>(
                widgets::SwitchProps{
                    .selector = [onBackPress]() { return onBackPress != nullptr; },
                    .evaluationMode = widgets::SwitchEvaluationMode::Lazy
                },
                std::make_unique<widgets::Switch<bool>::Branch>(
                    true,
                    std::make_unique<widgets::Button>(
                        widgets::ButtonProps{.onPress = onBackPress},
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