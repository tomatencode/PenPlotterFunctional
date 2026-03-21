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

    // measure() returns the minimum size needed to render the widget properly.
    virtual Size measure() const { return Size{0, 0}; }

    virtual bool canExpandHorizontally() const { return false; }
    virtual bool canExpandVertically() const { return false; }

    virtual bool isSelectable() const { return false; }

    virtual size_t childCount() const { return 0; }
    virtual Widget* child(size_t) const { return nullptr; }
};

} // namespace widgets
} // namespace ui
