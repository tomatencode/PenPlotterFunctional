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
          _props(std::move(props)),
          _child(std::move(child))
    {}

    void render(Renderer& r, Rect canvasBox) override {
        bool conditionValue = _props.condition();
        if (conditionValue != _lastConditionValue) {
            _lastConditionValue = conditionValue;
            
            if (!conditionValue) {
                // Condition is false, remove child
                setChild(nullptr);
            } else if (conditionValue && !getChild(0)) {
                // Condition is true and child is not set, add child back
                setChild(std::move(_child));
            }
        }
        Container::render(r, canvasBox);
    }

private:
    ConditionalProps _props;
    std::unique_ptr<Widget> _child;
    bool _lastConditionValue = false;
};

} // namespace widgets
} // namespace ui