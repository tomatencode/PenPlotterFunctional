#pragma once

#include <memory>
#include <vector>

#include "Widget.hpp"

namespace ui {
namespace widgets {

class ContainerWidget : public Widget
{
public:
    // Owning constructor: takes ownership of the widgets
    ContainerWidget(std::unique_ptr<Widget>&& child)
        : _child(std::move(child))
    {
    }

    ~ContainerWidget() = default;

    virtual size_t childCount() const override { return _child ? _child->childCount() : 0; }

    virtual Widget* child(size_t index) const override {
        if (_child)
            return _child->child(index);
        return nullptr;
    }

    virtual void render(Renderer& r, Rect canvasBox) override {
        if (_child)
            _child->render(r, canvasBox);
    }

    virtual Size measure() const override {
        return _child ? _child->measure() : Size{0, 0};
    }

    virtual Size desiredSize(const Size& available) const override {
        return _child ? _child->desiredSize(available) : Size{0, 0};
    }
private:
    std::unique_ptr<Widget> _child;
};

} // namespace widgets
} // namespace ui
