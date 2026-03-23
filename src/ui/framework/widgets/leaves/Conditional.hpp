#pragma once
#include "../core/Container.hpp"

namespace ui {
namespace widgets {

class ConditionalWidget : public Container
{
public:
    // Takes ownership of the child widget
    ConditionalWidget(std::function<bool()> en, std::unique_ptr<Widget> child)
        : Container(std::move(child)), _enabledCallback(en)
    {}

    void render(Renderer& r, Rect canvasBox) override
    {
        // shuld never happen be false
        if (_enabledCallback())
            Container::render(r, canvasBox);
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