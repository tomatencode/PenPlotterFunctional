#include "HeaderLine.hpp"
#include <vector>

HeaderLine::HeaderLine(const char* headerText, bool backButton,
                         std::function<void()> onBackPress)
    : Widget()
    , _wifi(nullptr)
    , _headerLabel(nullptr)
    , _backButton(nullptr)
    , _hasBackButton(backButton)
    , _layout(nullptr)
{
    // Create child widgets on the heap; ownership is transferred to the layout.
    _wifi = new WifiIndicator();
    _headerLabel = new LabelWidget(headerText);

    std::vector<std::unique_ptr<Widget>> children;
    children.emplace_back(std::unique_ptr<Widget>(_wifi));
    children.emplace_back(std::unique_ptr<Widget>(_headerLabel));

    if (_hasBackButton)
    {
        LabelWidget* backLabel = new LabelWidget("Back");
        _backButton = new ButtonWidget(std::unique_ptr<Widget>(backLabel), ButtonStyle(), onBackPress);
        children.emplace_back(std::unique_ptr<Widget>(_backButton));
    }

    LayoutStyle style = LayoutStyle();
    style.spacingMode = SpacingMode::SpaceBetween;
    _layout = new HorizontalLayout(std::move(children), style);
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
