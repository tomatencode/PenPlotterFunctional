#include "HeaderLine.hpp"
#include "../framework/widgets/Builder.hpp"

#include "../framework/widgets/layouts/HorizontalLayout.hpp"
#include "../framework/widgets/leaves/Label.hpp"
#include "../framework/widgets/leaves/Button.hpp"
#include "../framework/widgets/leaves/Spacer.hpp"

#include "WifiIndicator.hpp"


namespace ui {
namespace components {

HeaderLine::HeaderLine(const char* headerText, bool backButton,
                         std::function<void()> onBackPress)
    : Container(
        widgets::make_widget<widgets::HorizontalLayout>(
            widgets::HorizontalLayoutStyle{.spacingMode = widgets::SpacingMode::SpaceBetween},
            widgets::make_widget<components::WifiIndicator>(),
            widgets::make_widget<widgets::Label>(headerText),
            backButton
                ? widgets::make_widget<widgets::Button>(
                    widgets::make_widget<widgets::Label>("Back"),
                    widgets::ButtonStyle(),
                    onBackPress
                  )
                : widgets::make_widget<widgets::Spacer>(1, 1)
        )
    )
{
}

} // namespace components
} // namespace ui