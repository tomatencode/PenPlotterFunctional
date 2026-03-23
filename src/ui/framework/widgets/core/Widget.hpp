#pragma once

#include "../../render/Render.hpp"
#include "box.hpp"
#include "Size.hpp"

namespace ui {
namespace widgets {

class Widget
{
public:
    Widget() {}
    virtual ~Widget() = default;

    virtual void render(Renderer& r, Rect canvasBox) = 0;

    // minimum size needed to render the widget properly.
    virtual Size measure() const { return Size{0, 0}; }

    virtual bool canExpandHorizontally() const { return false; }
    virtual bool canExpandVertically() const { return false; }

    virtual bool isSelectable() const { return false; }

    virtual bool isEnabled() const {
        if (getParent())
            return getParent()->isEnabled();
        return true;
    }

    virtual void reload() {};

    virtual size_t childCount() const { return 0; }
    virtual Widget* child(size_t) const { return nullptr; }

    // for upstream widget tree traversal
    Widget* getParent() const { return _parent; }
    void setParent(Widget* parent) { _parent = parent; }
    
private:
    Widget* _parent = nullptr;
};

} // namespace widgets
} // namespace ui
