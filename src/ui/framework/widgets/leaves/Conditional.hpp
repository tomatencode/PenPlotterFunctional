#pragma once
#include "../core/Container.hpp"

namespace ui {
namespace widgets {

class Conditional : public Container
{
public:
    // Takes ownership of the child widget
    Conditional(std::function<bool()> en, bool lazy, std::unique_ptr<Widget> child)
        : Container(std::move(child)),
          _enabledCallback(en),
          _lazy(lazy)
    {}

    void render(Renderer& r, Rect canvasBox) override
    {
        // shuld never happen be false
        if (getCurrentEnabled())
            Container::render(r, canvasBox);
    }

    bool isEnabled() const override
    {
        return getCurrentEnabled();
    }

    void reload() override {
        Container::reload();
        // Clear cache to force re-evaluation of enabled state on next render/measure
        _current = false; // or some invalid state if T is not bool
    }

private:
    std::function<bool()> _enabledCallback;
    mutable bool _current;
    bool _lazy;

    bool getCurrentEnabled() const {
        if (!_current || !_lazy) {
            _current = _enabledCallback();
        }
        return _current;
    }
};

} // namespace widgets
} // namespace ui