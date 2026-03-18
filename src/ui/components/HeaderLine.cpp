#include "HeaderLine.hpp"
#include "../framework/widgets/Builder.hpp"

using namespace ui;

HeaderLine::HeaderLine(const char* headerText, bool backButton,
                         std::function<void()> onBackPress)
    : Widget()
    , _layout(nullptr)
{
    widgets::LayoutStyle style = widgets::LayoutStyle();
    style.spacingMode = widgets::SpacingMode::SpaceBetween;

    if (backButton)
    {
        _layout = widgets::make_layout<widgets::HorizontalLayout>(
            style,
            widgets::make_widget<WifiIndicator>(),
            widgets::make_widget<widgets::LabelWidget>(headerText),
            widgets::make_widget<widgets::ButtonWidget>(
                widgets::make_widget<widgets::LabelWidget>("Back"),
                widgets::ButtonStyle(),
                onBackPress
            )
        );
    }
    else
    {
        _layout = widgets::make_layout<widgets::HorizontalLayout>(
            style,
            widgets::make_widget<WifiIndicator>(),
            widgets::make_widget<widgets::LabelWidget>(headerText)
        );
    }
}

HeaderLine::~HeaderLine() = default;

widgets::Size HeaderLine::measure() const
{
    if (_layout)
        return _layout->measure();
    return {0, 0};
}

void HeaderLine::render(ui::Renderer& r, widgets::Rect canvasBox)
{
    if (!_layout)
        return;

    _layout->render(r, canvasBox);
}

size_t HeaderLine::childCount() const
{
    return _layout ? _layout->childCount() : 0;
}

widgets::Widget* HeaderLine::child(size_t index) const
{
    return _layout ? _layout->child(index) : nullptr;
}
