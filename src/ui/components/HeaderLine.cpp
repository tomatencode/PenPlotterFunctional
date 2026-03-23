#include "HeaderLine.hpp"
#include "../framework/widgets/Builder.hpp"

#include "../framework/widgets/layouts/HorizontalLayout.hpp"
#include "../framework/widgets/leaves/Label.hpp"
#include "../framework/widgets/leaves/Button.hpp"
#include "../framework/widgets/leaves/Spacer.hpp"

#include "WifiIndicator.hpp"


namespace ui {
namespace components {

using namespace widgets;

HeaderLine::HeaderLine(const char* headerText, bool backButton,
                         std::function<void()> onBackPress)
    : Container(
        make_layout<HorizontalLayout>(
            HorizontalLayoutStyle{.spacingMode = SpacingMode::SpaceBetween},
            make_widget<WifiIndicator>(),
            make_widget<Label>(headerText),
            backButton
                ? make_widget<Button>(
                    make_widget<Label>("Back"),
                    ButtonStyle(),
                    onBackPress
                  )
                : make_widget<Spacer>(1, 1)
        )
    )
{
}

} // namespace components
} // namespace ui