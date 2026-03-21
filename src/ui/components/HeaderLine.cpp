#include "HeaderLine.hpp"
#include "../framework/widgets/Builder.hpp"

#include "../framework/widgets/layouts/HorizontalLayout.hpp"
#include "../framework/widgets/leaves/LabelWidget.hpp"
#include "../framework/widgets/leaves/ButtonWidget.hpp"
#include "../framework/widgets/leaves/SpacerWidget.hpp"

#include "WifiIndicator.hpp"


namespace ui {
namespace components {

using namespace widgets;

HeaderLine::HeaderLine(const char* headerText, bool backButton,
                         std::function<void()> onBackPress)
    : ContainerWidget(
        make_layout<HorizontalLayout>(
            HorizontalLayoutStyle{.spacingMode = SpacingMode::SpaceBetween},
            make_widget<WifiIndicator>(),
            make_widget<LabelWidget>(headerText),
            backButton
                ? make_widget<ButtonWidget>(
                    make_widget<LabelWidget>("Back"),
                    ButtonStyle(),
                    onBackPress
                  )
                : make_widget<SpacerWidget>(1, 1)
        )
    )
{
}

} // namespace components
} // namespace ui