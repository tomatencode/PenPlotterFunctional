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

    virtual Size measure() const { return Size{0, 0}; }

    virtual bool isSelectable() const { return false; }

    virtual size_t childCount() const { return 0; }
    virtual Widget* child(size_t) const { return nullptr; }
};

} // namespace widgets
} // namespace ui
