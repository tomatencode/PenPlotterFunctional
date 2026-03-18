#include "HeaderLine.hpp"

HeaderLine::HeaderLine(const char* headerText, bool backButton,
                         std::function<void()> onBackPress)
    : Widget()
    , _headerLabel(headerText)
    , _backLabel("Back")
    , _backButton(&_backLabel, ButtonStyle(), onBackPress)
    , _hasBackButton(backButton)
    , _layout(nullptr)
{
    // Build child list for the layout: wifi + header text + optional back button
    Widget* children[3];
    size_t count = 0;

    children[count++] = &_wifi;
    children[count++] = &_headerLabel;
    if (_hasBackButton)
        children[count++] = &_backButton;

    LayoutStyle style = LayoutStyle();
    style.spacingMode = SpacingMode::SpaceBetween;
    _layout = new HorizontalLayout(children, count, style);
}

HeaderLine::~HeaderLine()
{
    delete _layout;
}

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
