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
    // desiredSize() can be overridden to specify a preferred size that may be larger than the minimum.
    virtual Size desiredSize(const Size& available) const { return measure(); }

    virtual bool isSelectable() const { return false; }

    virtual size_t childCount() const { return 0; }
    virtual Widget* child(size_t) const { return nullptr; }
};

} // namespace widgets
} // namespace ui
