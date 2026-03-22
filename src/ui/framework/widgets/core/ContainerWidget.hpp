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
        if (_child) _child->setParent(this);
    }

    ~ContainerWidget() = default;

    virtual size_t childCount() const override { return _child ? 1 : 0; }

    virtual Widget* child(size_t index) const override {
        if (_child && index == 0)
            return _child.get();
        return nullptr;
    }

    virtual void setChild(std::unique_ptr<Widget> newChild) {
        if (newChild)
            newChild->setParent(this);
        _child = std::move(newChild);
    }

    virtual void render(Renderer& r, Rect canvasBox) override {
        if (_child)
            _child->render(r, canvasBox);
    }

    virtual Size measure() const override {
        return _child ? _child->measure() : Size{0, 0};
    }

    virtual bool canExpandHorizontally() const override {
        return _child ? _child->canExpandHorizontally() : false;
    }

    virtual bool canExpandVertically() const override {
        return _child ? _child->canExpandVertically() : false;
    }
private:
    std::unique_ptr<Widget> _child;
};

} // namespace widgets
} // namespace ui
