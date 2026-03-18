#include "HeaderLine.hpp"
#include "../framework/widgets/Builder.hpp"

HeaderLine::HeaderLine(const char* headerText, bool backButton,
                         std::function<void()> onBackPress)
    : Widget()
    , _layout(nullptr)
{
    LayoutStyle style = LayoutStyle();
    style.spacingMode = SpacingMode::SpaceBetween;

    if (backButton)
    {
        _layout = ui::widgets::make_layout<HorizontalLayout>(
            style,
            ui::widgets::make_widget<WifiIndicator>(),
            ui::widgets::make_widget<LabelWidget>(headerText),
            ui::widgets::make_widget<ButtonWidget>(
                ui::widgets::make_widget<LabelWidget>("Back"),
                ButtonStyle(),
                onBackPress
            )
        );
    }
    else
    {
        _layout = ui::widgets::make_layout<HorizontalLayout>(
            style,
            ui::widgets::make_widget<WifiIndicator>(),
            ui::widgets::make_widget<LabelWidget>(headerText)
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

void HeaderLine::render(Renderer& r, Rect canvasBox)
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
