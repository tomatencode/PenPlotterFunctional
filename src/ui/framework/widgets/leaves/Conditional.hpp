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
          _lazy(lazy),
          _valid(false),
          _current(false)
    {}

    void render(Renderer& r, Rect canvasBox) override
    {
        // shuld never happen be false
        if (getCurrentEnabled())
            Container::render(r, canvasBox);
    }

    bool isEnabled() const override
    {
        return Container::isEnabled() && getCurrentEnabled();
    }

    void reload() override {
        Container::reload();
        _valid = false;
    }

private:
    std::function<bool()> _enabledCallback;
    mutable bool _current;
    mutable bool _valid;
    bool _lazy;

    bool getCurrentEnabled() const {
        if (!_lazy || !_valid) {
            _current = _enabledCallback();
            _valid = true;
        }
        return _current;
    }
};

} // namespace widgets
} // namespace ui