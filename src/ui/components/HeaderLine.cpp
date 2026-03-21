#include "HeaderLine.hpp"
#include "../framework/widgets/Builder.hpp"


namespace ui {
namespace components {

using namespace widgets;

HeaderLine::HeaderLine(const char* headerText, bool backButton,
                         std::function<void()> onBackPress)
    : Widget()
    , _layout(nullptr)
{
    HorizontalLayoutStyle style = HorizontalLayoutStyle();
    style.spacingMode = SpacingMode::SpaceBetween;

    if (backButton)
    {
        _layout = make_layout<HorizontalLayout>(
            style,
            make_widget<WifiIndicator>(),
            make_widget<LabelWidget>(headerText),
            make_widget<ButtonWidget>(
                make_widget<LabelWidget>("Back"),
                ButtonStyle(),
                onBackPress
            )
        );
    }
    else
    {
        _layout = make_layout<HorizontalLayout>(
            style,
            make_widget<WifiIndicator>(),
            make_widget<LabelWidget>(headerText),
            make_widget<SpacerWidget>(1, 1)
        );
    }
}

HeaderLine::~HeaderLine() = default;

Size HeaderLine::measure() const
{
    if (_layout)
        return _layout->measure();
    return {0, 0};
}

Size HeaderLine::desiredSize(const Size& available) const
{
    if (_layout)
        return _layout->desiredSize(available);
    return {0, 0};
}

void HeaderLine::render(ui::Renderer& r, Rect canvasBox)
{
    if (!_layout)
        return;

    _layout->render(r, canvasBox);
}

size_t HeaderLine::childCount() const
{
    return _layout ? _layout->childCount() : 0;
}

Widget* HeaderLine::child(size_t index) const
{
    return _layout ? _layout->child(index) : nullptr;
}

} // namespace components
} // namespace ui