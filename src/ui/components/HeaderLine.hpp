#pragma once

#include <functional>
#include <memory>
#include <concepts>

#include "../framework/widgets/core/Container.hpp"
#include "../framework/widgets/leaves/Label.hpp"
#include "../components/WifiIndicator.hpp"
#include "../framework/widgets/leaves/Button.hpp"
#include "../framework/widgets/leaves/Spacer.hpp"
#include "../framework/widgets/layouts/HorizontalLayout.hpp"
#include "../framework/widgets/Builder.hpp"

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
        widgets::make_widget<widgets::HorizontalLayout>(
            widgets::HorizontalLayoutStyle{.spacingMode = widgets::SpacingMode::SpaceBetween},
            widgets::make_widget<components::WifiIndicator>(WifiStatusProvider),
            widgets::make_widget<widgets::Label>(textProvider),
            onBackPress != nullptr
                ? widgets::make_widget<widgets::Button>(
                    "Back",
                    widgets::ButtonStyle(),
                    onBackPress
                  )
                : widgets::make_widget<widgets::Spacer>(1, 1)
        )
    )
    {}
};

} // namespace components
} // namespace ui