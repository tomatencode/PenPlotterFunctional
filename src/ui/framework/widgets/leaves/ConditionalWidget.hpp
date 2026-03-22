#pragma once
#include "../core/ContainerWidget.hpp"

namespace ui {
namespace widgets {

class ConditionalWidget : public ContainerWidget
{
public:
    // Takes ownership of the child widget.
    ConditionalWidget(std::function<bool()> en, std::unique_ptr<Widget> child)
        : ContainerWidget(std::move(child)), _enabledCallback(en)
    {}

    void render(Renderer& r, Rect canvasBox) override
    {
        // shuld never happen
        if (_enabledCallback())
            ContainerWidget::render(r, canvasBox);
    }

    bool isEnabled() const override
    {
        return _enabledCallback();
    }

private:
    std::function<bool()> _enabledCallback;
};

} // namespace widgets
} // namespace ui