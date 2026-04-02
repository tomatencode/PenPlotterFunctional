#pragma once

#include <functional>
#include <memory>

#include "../core/Widget.hpp"
#include "../core/Container.hpp"

namespace ui {
namespace widgets {

struct ConditionalProps
{
    std::function<bool()> condition;
};

class Conditional : public Container
{
public:
    Conditional(ConditionalProps props, std::unique_ptr<Widget> child)
        : Container(std::move(child)),
          _props(std::move(props))
    {}

    void render(Renderer& r, Rect canvasBox) override {
        if (_props.condition())
            Container::render(r, canvasBox);
    }

    Size measure() const override {
        if (_props.condition())
            return Container::measure();
        return Size{0, 0};
    }

    bool canExpandHorizontally() const override {
        if (_props.condition())
            return Container::canExpandHorizontally();
        return false;
    }

    bool canExpandVertically() const override {
        if (_props.condition())
            return Container::canExpandVertically();
        return false;
    }

    size_t getChildCount() const override {
        if (_props.condition())
            return Container::getChildCount();
        return 0;
    }

    Widget* getChild(size_t index) const override {
        if (_props.condition())
            return Container::getChild(index);
        return nullptr;
    }

private:
    ConditionalProps _props;
};

} // namespace widgets
} // namespace ui